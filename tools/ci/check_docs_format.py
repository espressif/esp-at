#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

import re
import sys
import os

def ESP_LOGI(x):
    print('\033[32m{}\033[0m'.format(x))

def ESP_LOGE(x):
    print('\033[31m{}\033[0m'.format(x))

def ESP_LOGW(x):
    print('\033[33m{}\033[0m'.format(x))

def is_in_code_block(line, in_code_block, expecting_code):
    """Determine whether the current line is inside a code block.

    in_code_block:  whether currently inside a code block.
    expecting_code: whether the next line is expected to be code content
                    (set True immediately after a code block marker is seen).

    RST code blocks may use any indentation (2, 3, 4+ spaces).  We therefore
    treat *any* leading whitespace as "indented" rather than requiring exactly
    four spaces.
    """
    stripped = line.strip()
    is_indented = bool(line) and line[0] in ' \t'

    # Code block start markers (RST format)
    if stripped == '::' or stripped.startswith('.. code-block::') or \
       stripped.startswith('.. code::') or stripped.startswith('```'):
        return False, True  # Expecting code on next line; current line is not yet code content

    # If expecting code and the current line is blank, keep expecting
    if expecting_code and not stripped:
        return False, True

    # If expecting code and the current line is indented (any amount), enter the code block
    if expecting_code and is_indented:
        return True, False

    # If expecting code but the current line is neither blank nor indented, cancel expectation
    if expecting_code and not is_indented:
        return False, False

    # Inside a code block:
    if in_code_block:
        if not stripped:
            return True, False  # Blank line: stay in code block
        if is_indented:
            return True, False  # Indented line: stay in code block
        return False, False     # Non-indented line: exit code block

    return False, False


# ---------------------------------------------------------------------------
# Rule 1: '您' usage
# ---------------------------------------------------------------------------

def check_ni_usage(file_path):
    """Rule 1: Check for '您' (formal 'you'); '你' should be used instead."""
    errors = []
    in_code_block = False
    expecting_code = False

    with open(file_path, 'r', encoding='utf-8') as f:
        for line_num, line in enumerate(f, start=1):
            in_code_block, expecting_code = is_in_code_block(line, in_code_block, expecting_code)
            if not in_code_block and not expecting_code and '您' in line:
                errors.append((line_num, line.rstrip(), "Use '你' instead of '您'"))
    return errors


# ---------------------------------------------------------------------------
# Rule 2: List punctuation consistency
# ---------------------------------------------------------------------------

def check_list_punctuation(file_path):
    """Rule 2: Check that all items in a list either end with punctuation or none do."""
    errors = []
    in_list = False
    list_items = []
    list_start_line = 0

    with open(file_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    in_code_block = False
    expecting_code = False
    for line_num, line in enumerate(lines, start=1):
        in_code_block, expecting_code = is_in_code_block(line, in_code_block, expecting_code)
        if in_code_block or expecting_code:
            continue

        stripped = line.strip()

        # Detect list start
        if stripped.startswith('.. list::'):
            in_list = True
            list_items = []
            list_start_line = line_num
            continue

        if in_list:
            if re.match(r'^\s+[-*]\s+', line):
                item_content = re.sub(r'^\s+[-*]\s+', '', line).strip()
                if item_content:
                    has_punctuation = bool(re.search(r'[。，、；：！？]$', item_content))
                    list_items.append((line_num, has_punctuation, item_content))
            elif stripped == '':
                continue
            elif not line.startswith(' ') or not re.match(r'^\s+[-*]\s+', line):
                # List ended; check punctuation consistency
                if len(list_items) > 1:
                    punct_flags = [has_punct for _, has_punct, _ in list_items]
                    if not all(punct_flags) and any(punct_flags):
                        for item_line, has_punct, content in list_items:
                            errors.append((
                                item_line,
                                content,
                                f'Inconsistent list punctuation: list starts at line {list_start_line}; '
                                f'some items have punctuation, some do not'
                            ))
                in_list = False
                list_items = []

    # Handle list at end of file
    if in_list and len(list_items) > 1:
        punct_flags = [has_punct for _, has_punct, _ in list_items]
        if not all(punct_flags) and any(punct_flags):
            for item_line, has_punct, content in list_items:
                errors.append((
                    item_line,
                    content,
                    f'Inconsistent list punctuation: list starts at line {list_start_line}; '
                    f'some items have punctuation, some do not'
                ))

    return errors


# ---------------------------------------------------------------------------
# Helpers: backtick ranges and fixed expressions
# ---------------------------------------------------------------------------

def find_backtick_pairs(line):
    """Find all backtick pairs and return a list of (start_pos, end_pos) tuples."""
    backtick_pairs = []
    backtick_positions = []
    i = 0
    while i < len(line):
        if line[i] == '`':
            start = i
            count = 1
            i += 1
            while i < len(line) and line[i] == '`':
                count += 1
                i += 1
            backtick_positions.append((start, count))
        else:
            i += 1

    # Pair backticks: match each opening run with the next closing run of equal or greater length
    i = 0
    while i < len(backtick_positions):
        start_pos, start_count = backtick_positions[i]
        j = i + 1
        while j < len(backtick_positions):
            end_pos, end_count = backtick_positions[j]
            if end_count >= start_count:
                backtick_pairs.append((start_pos, end_pos + end_count))
                i = j
                break
            j += 1
        i += 1

    return backtick_pairs


def is_in_backticks(line, pos):
    """Return True if position pos is inside a backtick span (RST inline code reference).

    Supported RST forms:
    - `content`              inline code (single backtick)
    - ``content``            inline code (double backtick)
    - :role:`content`        role-based reference
    - :role:`text <link>`    role-based reference with hyperlink
    """
    backtick_pairs = find_backtick_pairs(line)
    for start_pos, end_pos in backtick_pairs:
        if start_pos <= pos < end_pos:
            return True
    return False


def is_fixed_expression(line, pos):
    """Return True if the '(' at pos is part of a fixed word(number) expression.

    Fixed expression pattern: WORD(NUMBER), e.g. CWR(128), ECN(64), Urgent(32).
    These are exempt from the 'space before English left parenthesis' rule.
    """
    if pos == 0:
        return False

    # Walk backwards from pos-1 to find the start of the preceding word
    word_start = pos - 1
    while word_start >= 0 and (line[word_start].isalnum() or line[word_start] == '_'):
        word_start -= 1
    word_start += 1

    if word_start >= pos:
        return False

    word = line[word_start:pos]
    if not word or not word[0].isalpha():
        return False

    # Check whether the parenthesis content is purely numeric (possibly with spaces)
    depth = 1
    end_pos = pos + 1
    while end_pos < len(line) and depth > 0:
        if line[end_pos] == '(':
            depth += 1
        elif line[end_pos] == ')':
            depth -= 1
        end_pos += 1

    if depth == 0:
        content = line[pos + 1:end_pos - 1].strip()
        if re.match(r'^[0-9\s]+$', content):
            return True

    return False


# ---------------------------------------------------------------------------
# Rule 3: Punctuation usage (brackets and backtick-followed-by-parenthesis)
# ---------------------------------------------------------------------------

def check_punctuation_usage(file_path):
    """Rule 3: Check parenthesis and bracket usage in Chinese paragraphs."""
    errors = []
    in_code_block = False
    expecting_code = False

    with open(file_path, 'r', encoding='utf-8') as f:
        for line_num, line in enumerate(f, start=1):
            in_code_block, expecting_code = is_in_code_block(line, in_code_block, expecting_code)
            if in_code_block or expecting_code:
                continue

            # Skip list items (handled by check_list_punctuation)
            if re.match(r'^\s+[-*]\s+', line):
                continue

            # Skip RST directives and role markers
            if line.strip().startswith('.. ') or line.strip().startswith(':'):
                continue

            # Rule 3 (zh_CN): bold Chinese label with English colon, e.g. **命令:** → **命令：**
            # Pattern: **<text containing Chinese>:** where colon is the last char before **
            if re.search(r'[\u4e00-\u9fff]', line):
                for m in re.finditer(r'\*\*([^*]*[\u4e00-\u9fff][^*]*):\*\*', line):
                    if not is_in_backticks(line, m.start()):
                        inner = m.group(1)
                        errors.append((
                            line_num,
                            line.rstrip(),
                            f'Column {m.start() + 1}: use a Chinese colon (：) inside bold Chinese '
                            f'label: "**{inner}:**" → "**{inner}：**"'
                        ))

            # Rule 7: closing backtick immediately followed by Chinese left parenthesis requires a space
            backtick_pairs = find_backtick_pairs(line)
            for start_pos, end_pos in backtick_pairs:
                if end_pos < len(line):
                    check_pos = end_pos
                    while check_pos < len(line) and line[check_pos] == ' ':
                        check_pos += 1
                    if check_pos < len(line) and line[check_pos] == '（' and check_pos == end_pos:
                        errors.append((
                            line_num,
                            line.rstrip(),
                            f'Column {end_pos + 1}: closing backtick immediately followed by Chinese left '
                            f'parenthesis "（"; a space is required between them'
                        ))

            # Check Chinese parentheses （）
            for match in re.finditer(r'[（）]', line):
                bracket = match.group()
                pos = match.start()

                if is_in_backticks(line, pos):
                    continue

                if bracket == '（':
                    # Find the matching closing parenthesis
                    depth = 1
                    end_pos = pos + 1
                    while end_pos < len(line) and depth > 0:
                        if line[end_pos] == '（':
                            depth += 1
                        elif line[end_pos] == '）':
                            depth -= 1
                        end_pos += 1

                    if depth == 0:
                        content = line[pos + 1:end_pos - 1]
                        # If the content is entirely ASCII/digits, English parentheses should be used
                        if re.match(r'^[0-9A-Za-z\s\-_\.]+$', content):
                            errors.append((
                                line_num,
                                line.rstrip(),
                                f"Column {pos + 1}: content '{content}' is all ASCII/digits; "
                                f'use English parentheses () instead of （）'
                            ))

            # Check English parentheses ()
            for match in re.finditer(r'[()]', line):
                bracket = match.group()
                pos = match.start()

                if is_in_backticks(line, pos):
                    continue

                if bracket == '(':
                    # Find the matching closing parenthesis
                    depth = 1
                    end_pos = pos + 1
                    while end_pos < len(line) and depth > 0:
                        if line[end_pos] == '(':
                            depth += 1
                        elif line[end_pos] == ')':
                            depth -= 1
                        end_pos += 1

                    if depth == 0:
                        content = line[pos + 1:end_pos - 1]
                        has_chinese = bool(re.search(r'[\u4e00-\u9fff]', content))

                        # If content contains Chinese, Chinese parentheses should be used
                        if has_chinese:
                            errors.append((
                                line_num,
                                line.rstrip(),
                                f"Column {pos + 1}: content '{content}' contains Chinese; "
                                f'use Chinese parentheses （） instead of ()'
                            ))

                        # Check spacing around the English left parenthesis.
                        # In an English context a space is required before '(' unless:
                        #   - preceded by punctuation or the line start
                        #   - it is a word+(s) plural form (e.g. parameter(s))     [Rule 6]
                        #   - it is a fixed expression like CWR(128)               [Rule 8]
                        if pos > 0:
                            prev_char = line[pos - 1]
                            if re.match(r'[A-Za-z0-9]', prev_char):
                                if content.strip() == 's':
                                    pass  # word+(s) plural; no space required
                                elif is_fixed_expression(line, pos):
                                    pass  # fixed expression; no space required
                                else:
                                    errors.append((
                                        line_num,
                                        line.rstrip(),
                                        f'Column {pos + 1}: a space is required before the English left parenthesis'
                                    ))

                        # A space is required after ')' when followed by text
                        # (not punctuation, space, or end of line)
                        if end_pos < len(line):
                            next_char = line[end_pos]
                            if re.match(r'[A-Za-z0-9\u4e00-\u9fff]', next_char):
                                errors.append((
                                    line_num,
                                    line.rstrip(),
                                    f'Column {end_pos + 1}: a space is required after the English right parenthesis '
                                    f'(unless followed by punctuation)'
                                ))

    return errors


# ---------------------------------------------------------------------------
# Rule 9: menuconfig navigation arrows
# ---------------------------------------------------------------------------

def check_menuconfig_arrows(file_path):
    """Rule 9: Check menuconfig navigation arrow usage.

    Requirements:
    - In menuconfig-related content (lines containing backticks), use '>' to connect
      steps, not '->' or '-->'.
    - '->' is acceptable in data-flow contexts (e.g. 'S1 -> S2 -> S3').
    - '->' is acceptable when indicating a configuration value change (e.g. '1->3').
    """
    errors = []
    in_code_block = False
    expecting_code = False

    with open(file_path, 'r', encoding='utf-8') as f:
        for line_num, line in enumerate(f, start=1):
            in_code_block, expecting_code = is_in_code_block(line, in_code_block, expecting_code)
            if in_code_block or expecting_code:
                continue

            if re.match(r'^\s+[-*]\s+', line):
                continue

            if line.strip().startswith('.. ') or line.strip().startswith(':'):
                continue

            if 'menuconfig' in line.lower() and '`' in line:
                checked_positions = set()

                # Check '-->' first to avoid double-reporting the embedded '->'
                for match in re.finditer(r'-->', line):
                    pos = match.start()
                    if pos in checked_positions:
                        continue
                    checked_positions.add(pos)
                    checked_positions.add(pos + 1)

                    before = line[max(0, pos - 20):pos].strip()
                    after = line[match.end():min(len(line), match.end() + 20)].strip()

                    is_data_flow = bool(re.search(r'(数据流|流|flow|stream)', before + after, re.IGNORECASE))
                    is_config_value = bool(re.search(
                        r'\d+\s*[-=]>\s*\d+',
                        line[max(0, pos - 10):min(len(line), match.end() + 10)]
                    ))

                    if not is_data_flow and not is_config_value:
                        errors.append((
                            line_num,
                            line.rstrip(),
                            f"Column {pos + 1}: use '>' instead of '-->' for menuconfig navigation"
                        ))

                # Then check '->'
                for match in re.finditer(r'->', line):
                    pos = match.start()
                    if pos in checked_positions:
                        continue
                    checked_positions.add(pos)

                    before = line[max(0, pos - 20):pos].strip()
                    after = line[match.end():min(len(line), match.end() + 20)].strip()

                    is_data_flow = bool(re.search(r'(数据流|流|flow|stream)', before + after, re.IGNORECASE))
                    is_config_value = bool(re.search(
                        r'\d+\s*[-=]>\s*\d+',
                        line[max(0, pos - 10):min(len(line), match.end() + 10)]
                    ))

                    if not is_data_flow and not is_config_value:
                        errors.append((
                            line_num,
                            line.rstrip(),
                            f"Column {pos + 1}: use '>' instead of '->' for menuconfig navigation"
                        ))

    return errors


# ---------------------------------------------------------------------------
# Rule 10: 'bit' formatting
# ---------------------------------------------------------------------------

def check_bit_usage(file_path):
    """Rule 10: Check 'bit' formatting.

    Requirements:
    1. When referring to a specific bit position, 'bit' must be followed by a space,
       e.g. 'bit 0'. Both 'Bit0' and 'bit0' are incorrect.
    2. In Chinese docs, 'Bit' must be lowercased to 'bit'.
    3. In English docs, sentence-initial 'Bit 0' is acceptable.
       However, in list items 'bit 0:' must use a lowercase 'b'.
    """
    errors = []
    in_code_block = False
    expecting_code = False
    is_zh_cn = 'zh_CN' in file_path

    with open(file_path, 'r', encoding='utf-8') as f:
        for line_num, line in enumerate(f, start=1):
            in_code_block, expecting_code = is_in_code_block(line, in_code_block, expecting_code)
            if in_code_block or expecting_code:
                continue

            if line.strip().startswith('.. ') or line.strip().startswith(':'):
                continue

            is_list_line = bool(re.match(r'^\s*[\*\-]\s+', line))

            # In list items, 'Bit N:' must be 'bit N:' (both CN and EN)
            if is_list_line:
                for match in re.finditer(r'\bBit\s+(\d+)\s*:', line):
                    pos = match.start()
                    if is_in_backticks(line, pos):
                        continue
                    num = match.group(1)
                    errors.append((
                        line_num,
                        line.rstrip(),
                        f"Column {pos + 1}: use lowercase 'bit {num}:' in list items, not 'Bit {num}:'"
                    ))
                    break

            # Chinese docs: any 'Bit N' must be changed to 'bit N'
            if is_zh_cn:
                for match in re.finditer(r'\bBit\s+\d+', line):
                    pos = match.start()
                    if is_in_backticks(line, pos):
                        continue
                    if is_list_line and re.match(r'\s*:', line[match.end():]):
                        continue  # Already reported by the list-item check above
                    errors.append((
                        line_num,
                        line.rstrip(),
                        f"Column {pos + 1}: use lowercase 'bit {match.group(0).split()[-1]}' "
                        f"in Chinese docs, not '{match.group(0)}'"
                    ))

            # 'bit' immediately followed by a digit (no space): bit0, Bit0, etc.
            for match in re.finditer(r'\b[bB][iI][tT](\d+)', line):
                pos = match.start()
                if is_in_backticks(line, pos):
                    continue

                before = line[:pos].strip()
                is_sentence_start = (
                    not before
                    or before.endswith('.')
                    or before.endswith('!')
                    or before.endswith('?')
                )

                if is_sentence_start and line[pos] == 'B' and not is_zh_cn:
                    if pos + 3 < len(line) and line[pos + 3] == ' ':
                        continue

                errors.append((
                    line_num,
                    line.rstrip(),
                    f"Column {pos + 1}: 'bit' must be followed by a space; "
                    f"use 'bit {match.group(1)}' instead of '{match.group(0)}'"
                ))

    return errors


# ---------------------------------------------------------------------------
# Rule 11: Range symbols
# ---------------------------------------------------------------------------

def check_range_symbols(file_path):
    """Rule 11: Check range symbol usage.

    Requirements:
    1. In Chinese: use '～' (fullwidth tilde U+FF5E), e.g. 'bit 23 ～ bit 16'.
    2. In English: use '–' (en dash U+2013), e.g. 'bit 23 – bit 16'.
    3. When both sides of the range are plain numbers, surrounding spaces are not required,
       e.g. 'bit 23–16'.
    4. When either side contains non-digit content, a space before the symbol is required,
       e.g. 'bit 23 – bit 16'.
    5. If neither side of the symbol contains a digit, it is not treated as a range and
       is skipped (e.g. '工程配置 - 主窗口').
    """
    errors = []
    in_code_block = False
    expecting_code = False

    with open(file_path, 'r', encoding='utf-8') as f:
        for line_num, line in enumerate(f, start=1):
            in_code_block, expecting_code = is_in_code_block(line, in_code_block, expecting_code)
            if in_code_block or expecting_code:
                continue

            if re.match(r'^\s+[-*]\s+', line):
                continue

            if line.strip().startswith('.. ') or line.strip().startswith(':'):
                continue

            # Chinese text: should use ～, not -, –, —, or ~
            chinese_range_patterns = [
                (r'[\u4e00-\u9fff]+\s*[-~—]\s*[\u4e00-\u9fff]+', 'Chinese ranges should use ～'),
                (r'[\u4e00-\u9fff]+\s*–\s*[\u4e00-\u9fff]+',     'Chinese ranges should use ～ instead of –'),
            ]

            for pattern, msg in chinese_range_patterns:
                for match in re.finditer(pattern, line):
                    pos = match.start()
                    if is_in_backticks(line, pos):
                        continue

                    match_text = match.group(0)
                    symbol_match = re.search(r'[-~—–]', match_text)
                    if symbol_match:
                        before_symbol = match_text[:symbol_match.start()]
                        after_symbol  = match_text[symbol_match.start() + len(symbol_match.group()):]
                        # Skip if neither side has a digit (not a numeric range)
                        if not re.search(r'\d', before_symbol) or not re.search(r'\d', after_symbol):
                            continue

                    if re.search(r'[\u4e00-\u9fff]', match_text):
                        if '～' not in match_text and (
                            '-' in match_text or '–' in match_text
                            or '—' in match_text or '~' in match_text
                        ):
                            errors.append((line_num, line.rstrip(), f'Column {pos + 1}: {msg}'))

            # 'bit N <symbol> bit N' pattern
            for match in re.finditer(r'\bbit\s+\d+\s*([-~—–])\s*bit\s+\d+', line, re.IGNORECASE):
                pos = match.start()
                symbol = match.group(1)
                if is_in_backticks(line, pos):
                    continue

                context = line[max(0, pos - 30):min(len(line), match.end() + 30)]
                has_chinese = bool(re.search(r'[\u4e00-\u9fff]', context))

                if has_chinese:
                    if symbol != '～':
                        errors.append((
                            line_num, line.rstrip(),
                            f"Column {pos + 1}: Chinese range should use '～' instead of '{symbol}'"
                        ))
                else:
                    if symbol != '–':
                        errors.append((
                            line_num, line.rstrip(),
                            f"Column {pos + 1}: English range should use '–' (en dash U+2013) instead of '{symbol}'"
                        ))

            # 'bit N<symbol>N' pattern (plain-number range, no space needed)
            for match in re.finditer(r'\bbit\s+(\d+)([-~—–])(\d+)', line, re.IGNORECASE):
                pos = match.start()
                symbol = match.group(2)
                if is_in_backticks(line, pos):
                    continue

                context = line[max(0, pos - 30):min(len(line), match.end() + 30)]
                has_chinese = bool(re.search(r'[\u4e00-\u9fff]', context))

                if has_chinese:
                    if symbol != '～':
                        errors.append((
                            line_num, line.rstrip(),
                            f"Column {pos + 1}: Chinese range should use '～' instead of '{symbol}'"
                        ))
                else:
                    if symbol != '–':
                        errors.append((
                            line_num, line.rstrip(),
                            f"Column {pos + 1}: English range should use '–' (en dash U+2013) instead of '{symbol}'"
                        ))

    return errors


# ---------------------------------------------------------------------------
# Rule 12: Colon usage in Chinese doc parameter lists
# ---------------------------------------------------------------------------

def check_param_list_colon(file_path):
    """Rule 12: Check colon usage in Chinese doc lists (zh_CN files only).

    Requirements:
    - :ref: links followed by Chinese text must use a Chinese colon (：), not (:).
    - Parameter names (**<xxx>**) should be followed by a Chinese colon (：).
    - Bit flags ('bit N') and numeric enum values should be followed by an English colon (:).
    - Error: :ref: link + English colon + Chinese description,
             e.g. ':ref:`AT+CIPV6 <cmd-IPV6>`: 启用或禁用 IPv6 网络'.
    - Error: enum value followed by Chinese colon, e.g. '0：OPEN'.
    - Error: 'bit N' followed by Chinese colon, e.g. 'bit 0：description'.
    - Error: parameter name followed by English colon, e.g. '**<"mac">**: string param'.
    """
    if 'zh_CN' not in file_path:
        return []

    errors = []
    in_code_block = False
    expecting_code = False

    with open(file_path, 'r', encoding='utf-8') as f:
        for line_num, line in enumerate(f, start=1):
            in_code_block, expecting_code = is_in_code_block(line, in_code_block, expecting_code)
            if in_code_block or expecting_code:
                continue

            # Check A (any line): :ref: link + English colon + Chinese text.
            # Applies to both list items and standalone :ref: title lines.
            # e.g. '- :ref:`AT+CIPV6 <cmd-IPV6>`: 启用或禁用 IPv6 网络'
            #   or ':ref:`AT+CIPV6 <TCPIP-AT>`: 启用或禁用 IPv6 网络'
            for ref_m in re.finditer(r':ref:`[^`]+`\s*:\s*(.*)', line):
                rest = ref_m.group(1)
                if re.search(r'[\u4e00-\u9fff]', rest):
                    errors.append((
                        line_num,
                        line.rstrip(),
                        'Use a Chinese colon (：) after :ref: link when followed by '
                        'Chinese text, not an English colon (:)'
                    ))
                    break  # one error per line is enough

            # The checks below only apply to list items.
            list_match = re.match(r'^\s*[\*\-]\s+(.*)$', line)
            if not list_match:
                continue

            content = list_match.group(1).strip()

            # Check B: Numeric enum value + Chinese colon: e.g. '0：OPEN'
            match_enum_cn = re.match(r'^(\d+)\s*：\s*', content)
            if match_enum_cn:
                errors.append((
                    line_num,
                    line.rstrip(),
                    f'Use an English colon after enum values, e.g. '
                    f"'{match_enum_cn.group(1)}: ...' not '{match_enum_cn.group(1)}：'"
                ))
                continue

            # Check C: 'bit N' + Chinese colon: e.g. 'bit 0：description'
            match_bit_cn = re.search(r'bit\s+\d+\s*：', content)
            if match_bit_cn:
                errors.append((
                    line_num,
                    line.rstrip(),
                    "Use an English colon (:) after 'bit N', not a Chinese colon (：)"
                ))
                continue

            # Check D: Parameter name (**<xxx>**) followed by an English colon:
            # should use Chinese colon.
            match_param_en = re.match(r'^\*\*([^*]+)\*\*\s*:\s*', content)
            if match_param_en:
                inner = match_param_en.group(1)
                if '<' in inner or '>' in inner or '"' in inner or "'" in inner:
                    errors.append((
                        line_num,
                        line.rstrip(),
                        'Use a Chinese colon (：) after a parameter name, not an English colon (:)'
                    ))

    return errors


# ---------------------------------------------------------------------------
# Rule 13: Enum value capitalization in English docs
# ---------------------------------------------------------------------------

def check_enum_capitalization(file_path):
    """Rule 13: Check that the text after a numeric enum value in English docs starts with
    a capital letter, but only when the text is a sentence (ends with a period).

    Only applies to English docs (not zh_CN).
    Pattern: list item starting with 'N: lowercase_letter...'
    Words or phrases without a trailing period are exempt.
    """
    if 'zh_CN' in file_path:
        return []

    errors = []
    in_code_block = False
    expecting_code = False

    with open(file_path, 'r', encoding='utf-8') as f:
        for line_num, line in enumerate(f, start=1):
            in_code_block, expecting_code = is_in_code_block(line, in_code_block, expecting_code)
            if in_code_block or expecting_code:
                continue

            list_match = re.match(r'^\s*[\*\-]\s+(.*)$', line)
            if not list_match:
                continue

            content = list_match.group(1).strip()

            # Match: number + colon + space + lowercase letter
            match = re.match(r'^(\d+)\s*:\s*([a-z])(.*)', content)
            if match:
                rest = match.group(2) + match.group(3)
                # Only flag if the text is a sentence (ends with a period)
                if rest.rstrip().endswith('.'):
                    errors.append((
                        line_num,
                        line.rstrip(),
                        f"The text after enum value '{match.group(1)}:' is a sentence and "
                        f'should start with a capital letter'
                    ))

    return errors


# ---------------------------------------------------------------------------
# Rule 14: Spacing
# ---------------------------------------------------------------------------

def _find_table_line_set(lines):
    """Return the set of 0-based line indices that belong to RST simple tables.

    RST simple tables are delimited by border rows of the form:
      ========= ============ ==========
    (two or more runs of '=' separated by spaces).

    All lines between the first and last border row of a table group (inclusive)
    are considered part of the table and exempt from the spacing check.
    Groups of borders within 50 lines of each other are treated as one table.
    """
    border_idxs = []
    for i, line in enumerate(lines):
        if re.match(r'^\s*={2,}(\s+={2,})+\s*$', line.rstrip('\n')):
            border_idxs.append(i)

    table_idxs = set()
    i = 0
    while i < len(border_idxs):
        group_end = border_idxs[i]
        j = i + 1
        # Merge consecutive borders within 50 lines into a single table group
        while j < len(border_idxs) and border_idxs[j] - group_end <= 50:
            group_end = border_idxs[j]
            j += 1
        for idx in range(border_idxs[i], group_end + 1):
            table_idxs.add(idx)
        i = j

    return table_idxs


def check_spacing(file_path):
    """Rule 14: Check spacing.

    Requirements:
    - No whitespace characters on otherwise blank lines.
    - No trailing whitespace at the end of lines.
    - List markers (- or *) must be followed by exactly one space before content.
      e.g. '-  0: ...' should be '- 0: ...'
    - No multiple consecutive spaces within line content (outside backticks).
      e.g. 'enter  Network' should be 'enter Network'.

    Exemptions:
    - RST comment lines (line starts with '..') are skipped for list-marker and
      inline-space checks, because spacing after '..' is part of RST comment syntax.
    - RST simple table lines (between '=====' border rows) are skipped for
      list-marker and inline-space checks, because column alignment uses spaces.
    - Code blocks are already skipped via is_in_code_block().
    """
    errors = []
    in_code_block = False
    expecting_code = False

    with open(file_path, 'r', encoding='utf-8') as f:
        all_lines = f.readlines()

    table_line_set = _find_table_line_set(all_lines)

    for line_num, line in enumerate(all_lines, start=1):
        in_code_block, expecting_code = is_in_code_block(line, in_code_block, expecting_code)

        raw_line = line.rstrip('\n')

        # Check for trailing whitespace (applies everywhere, including code blocks)
        if raw_line != raw_line.rstrip():
            errors.append((
                line_num,
                raw_line,
                'Trailing whitespace at end of line'
            ))
            continue

        if not in_code_block and not expecting_code:
            # Check for spaces on otherwise blank lines
            if raw_line and not raw_line.strip():
                errors.append((
                    line_num,
                    repr(raw_line),
                    'Blank line contains whitespace; it should be completely empty'
                ))
                continue

            # RST comment lines (starting with '..') use spacing as part of
            # the comment syntax; skip list-marker and inline-space checks.
            if raw_line.lstrip().startswith('..'):
                continue

            # RST simple table lines use spaces for column alignment;
            # skip list-marker and inline-space checks.
            if (line_num - 1) in table_line_set:
                continue

            # Check for extra spaces after list markers (- or *)
            # e.g. '-  0: ...' → '- 0: ...'
            if re.match(r'^\s*[-*]\s{2,}\S', raw_line):
                errors.append((
                    line_num,
                    raw_line,
                    'Extra space(s) after list marker; use a single space between the marker and content'
                ))
                continue

            # Check for inline multiple consecutive spaces (outside backticks).
            # Start scanning from after the list marker (or after leading indentation).
            lm = re.match(r'^(\s*[-*]\s+)', raw_line)
            content_start = lm.end() if lm else len(raw_line) - len(raw_line.lstrip())
            for m in re.finditer(r' {2,}', raw_line[content_start:]):
                actual_pos = content_start + m.start()
                if not is_in_backticks(raw_line, actual_pos):
                    errors.append((
                        line_num,
                        raw_line,
                        f'Column {actual_pos + 1}: multiple consecutive spaces; use a single space'
                    ))
                    break  # Report once per line to avoid noise

    return errors


# ---------------------------------------------------------------------------
# Rule 15: Line count consistency between CN and EN docs
# ---------------------------------------------------------------------------

_LINE_COUNT_SKIP_SUFFIXES = (
    'docs/zh_CN/index_of_abbreviations.rst',
    'docs/en/index_of_abbreviations.rst',
    'docs/zh_CN/Compile_and_Develop/AT_API_Reference.rst',
    'docs/en/Compile_and_Develop/AT_API_Reference.rst',
    'docs/zh_CN/Customized_AT_Commands_and_Firmware/RainMaker_AT/index_of_abbreviations.rst',
    'docs/en/Customized_AT_Commands_and_Firmware/RainMaker_AT/index_of_abbreviations.rst',
)


def check_line_count_consistency(file_path):
    """Rule 15: Check that the CN and EN versions of the same document have the same
    number of lines.

    The following files are exempt from this check:
      - docs/zh_CN/index_of_abbreviations.rst
      - docs/en/index_of_abbreviations.rst
      - docs/zh_CN/Compile_and_Develop/AT_API_Reference.rst
      - docs/en/Compile_and_Develop/AT_API_Reference.rst
    """
    normalized = file_path.replace(os.sep, '/')
    if any(normalized.endswith(suffix) for suffix in _LINE_COUNT_SKIP_SUFFIXES):
        return []

    if 'zh_CN' in file_path:
        counterpart = file_path.replace('zh_CN', 'en')
    elif '/en/' in file_path:
        counterpart = file_path.replace('/en/', '/zh_CN/')
    else:
        return []

    if not os.path.exists(counterpart):
        return []

    with open(file_path, 'r', encoding='utf-8') as f:
        line_count = sum(1 for _ in f)

    with open(counterpart, 'r', encoding='utf-8') as f:
        counterpart_count = sum(1 for _ in f)

    if line_count != counterpart_count:
        return [(
            1,
            file_path,
            f'Line count mismatch: this file has {line_count} lines, but its counterpart '
            f'({counterpart}) has {counterpart_count} lines'
        )]

    return []


# ---------------------------------------------------------------------------
# Rule 16: Parameter description format (square brackets and leading commas)
# ---------------------------------------------------------------------------

def check_param_brackets(file_path):
    """Rule 16: Check parameter description format.

    Requirements:
    - Remove outer square brackets from the bold parameter-name label, e.g.:
        '**[<port>]**:'   → '**<port>**:'
    - Remove leading commas inside angle brackets, e.g.:
        '**<,CA enable>**:' → '**<CA enable>**:'
    """
    errors = []
    in_code_block = False
    expecting_code = False

    with open(file_path, 'r', encoding='utf-8') as f:
        for line_num, line in enumerate(f, start=1):
            in_code_block, expecting_code = is_in_code_block(line, in_code_block, expecting_code)
            if in_code_block or expecting_code:
                continue

            list_match = re.match(r'^\s*[\*\-]\s+(.*)$', line)
            if not list_match:
                continue

            content = list_match.group(1).strip()

            # '**[<param>]**' — outer square brackets should be removed
            match_brackets = re.match(r'^\*\*\[(<[^>]+>)\]\*\*', content)
            if match_brackets:
                inner = match_brackets.group(1)
                errors.append((
                    line_num,
                    line.rstrip(),
                    f'Remove outer square brackets from the parameter name: '
                    f"use '**{inner}**' instead of '**[{inner}]**'"
                ))
                continue

            # '**<,param>**' — leading comma inside angle brackets should be removed
            match_comma = re.match(r'^\*\*<,([^>]+)>\*\*', content)
            if match_comma:
                inner = match_comma.group(1)
                errors.append((
                    line_num,
                    line.rstrip(),
                    f'Remove the leading comma from the parameter name: '
                    f"use '**<{inner}>**' instead of '**<,{inner}>**'"
                ))

    return errors


# ---------------------------------------------------------------------------
# Rule 2 (EN) / formerly Rule 17: List sentence format and first-letter capitalisation
# ---------------------------------------------------------------------------

def _is_rst_heading_underline(line):
    """Return True if *line* is an RST section-title underline.

    An underline is a non-empty line composed entirely of the same repeated
    RST adornment character (e.g. '----', '====', '~~~~').
    """
    stripped = line.strip()
    return bool(stripped) and bool(re.match(r'^([!"#$%&\'()*+,\-./:;<=>?@\[\\\]^_{|}~])\1{2,}$', stripped))


def _description_is_code_only(description):
    """Return True if the description contains only backtick code spans (no plain text).

    e.g. '``AT+UART_DEF=115200,8,1,0,3``' is code-only and should be skipped.
    """
    # Remove all backtick spans (single or double) and check if anything remains.
    stripped = re.sub(r'``[^`]+``', '', description)
    stripped = re.sub(r'`[^`]+`', '', stripped)
    return not stripped.strip()


_ALLOWED_SLASH_WORDS = frozenset({
    # All-caps abbreviations are handled dynamically (p.isupper()); the words
    # below are mixed-case proper nouns that are explicitly allowed by the
    # style guide: "TCP/UDP/SSL TCP/IP TCP/SSL Station/SoftAP/Station+SoftAP"
    'Station',
    'SoftAP',
})


def _has_non_exempt_slash(description):
    """Return True if *description* contains a '/' used to mean 'or'.

    Exempt patterns:
    - Slash between all-uppercase abbreviations (e.g. TCP/IP, TCP/UDP/SSL).
    - Slash between known proper-noun tech terms (e.g. Station/SoftAP).
    Only flag when a token around the slash is neither all-uppercase nor in
    the explicit allowed-words set.
    """
    for match in re.finditer(r'[A-Za-z]+(?:/[A-Za-z]+)+', description):
        parts = match.group(0).split('/')
        if not all(p.isupper() or p in _ALLOWED_SLASH_WORDS for p in parts):
            return True
    return False


def check_list_sentence(file_path):
    """Rule 2 (EN): Check list sentence format in English docs.

    For lines (in a list or standalone) containing a :ref: link followed by
    a description, e.g.:
      - :ref:`AT+CMD <ANCHOR>`: Description text here.

    Requirements:
    - The description must start with an uppercase letter.
    - The description must end with a period '.'.
    - Do not use '/' to mean 'or' between words; use 'or' instead.

    Exemptions:
    - Description that consists entirely of backtick code spans is skipped
      (e.g. - :ref:`AT+UART_DEF <cmd-UARTD>`: ``AT+UART_DEF=115200,8,1,0,3``).
    - Slash between all-uppercase abbreviations is allowed
      (e.g. TCP/IP, TCP/UDP/SSL, TCP/SSL).
    - Section-title lines (a :ref: line immediately followed by an RST heading
      underline) are exempt from the period check, but the '/' check still applies.
    """
    if 'zh_CN' in file_path:
        return []

    errors = []
    in_code_block = False
    expecting_code = False

    with open(file_path, 'r', encoding='utf-8') as f:
        all_lines = f.readlines()

    for idx, line in enumerate(all_lines):
        line_num = idx + 1
        in_code_block, expecting_code = is_in_code_block(line, in_code_block, expecting_code)
        if in_code_block or expecting_code:
            continue

        # Extract content (strip list marker if present)
        list_match = re.match(r'^\s*[-*]\s+(.*)', line)
        content = list_match.group(1) if list_match else line.strip()

        # Match :ref:`...`: description at the start of content
        ref_match = re.match(r'^:ref:`[^`]+`\s*:\s*(.+)', content)
        if not ref_match:
            continue

        description = ref_match.group(1).rstrip()

        # Exemption: description is only code spans — skip all checks.
        if _description_is_code_only(description):
            continue

        # Detect section-title lines: the immediately following non-blank line
        # is an RST heading underline.
        next_non_blank = ''
        for j in range(idx + 1, len(all_lines)):
            if all_lines[j].strip():
                next_non_blank = all_lines[j]
                break
        is_section_title = _is_rst_heading_underline(next_non_blank)

        # Check 1: first character of description must be uppercase
        if description and description[0].islower():
            errors.append((
                line_num,
                line.rstrip(),
                'List description should start with an uppercase letter'
            ))

        # Check 2: description must end with a period
        # (skipped for section-title lines)
        if not is_section_title and not description.endswith('.'):
            errors.append((
                line_num,
                line.rstrip(),
                "List description must end with a period '.'"
            ))

        # Check 3: do not use '/' to mean 'or' between words
        # (applies to all lines, including section-title lines;
        #  all-uppercase abbreviation slashes are exempt)
        if _has_non_exempt_slash(description):
            errors.append((
                line_num,
                line.rstrip(),
                "Use 'or' instead of '/' to connect two words in a description"
            ))

    return errors


# ---------------------------------------------------------------------------
# Top-level: run all checks on a single file
# ---------------------------------------------------------------------------

def check_file(file_path):
    """Run all checks on a single file and return a list of (category, error) tuples."""
    all_errors = []

    checks = [
        ('ni-usage',          check_ni_usage),               # Rule 1
        ('list-punctuation',  check_list_punctuation),       # Rule 2 (CN)
        ('list-sentence',     check_list_sentence),          # Rule 2 (EN)
        ('punctuation',       check_punctuation_usage),      # Rule 3
        ('menuconfig-arrows', check_menuconfig_arrows),      # Rule 9
        ('bit-format',        check_bit_usage),              # Rule 10
        ('range-symbols',     check_range_symbols),          # Rule 11
        ('param-list-colon',  check_param_list_colon),       # Rule 12
        ('enum-capital',      check_enum_capitalization),    # Rule 13
        ('spacing',           check_spacing),                # Rule 14
        ('line-count',        check_line_count_consistency), # Rule 15
        ('param-brackets',    check_param_brackets),         # Rule 16
    ]

    for category, check_fn in checks:
        errs = check_fn(file_path)
        if errs:
            all_errors.extend([(category, err) for err in errs])

    return all_errors


def main():
    """Entry point."""
    if len(sys.argv) < 2:
        ESP_LOGE('Usage: {} <file_path>...'.format(sys.argv[0]))
        sys.exit(1)

    all_passed = True

    for file_path in sys.argv[1:]:
        if not os.path.exists(file_path):
            ESP_LOGW('File not found: {}'.format(file_path))
            continue

        if not file_path.endswith('.rst'):
            continue

        errors = check_file(file_path)

        if errors:
            all_passed = False
            ESP_LOGE('Format issues found in {}:'.format(file_path))

            current_type = None
            for err_type, (line_num, content, msg) in errors:
                if current_type != err_type:
                    current_type = err_type
                    print('\n[{}]'.format(err_type))
                print('  Line {}: {}'.format(line_num, msg))
                print('  {}'.format(content))
                print()

    if all_passed:
        ESP_LOGI('Documentation format check passed!')
        return 0
    else:
        return 1


if __name__ == '__main__':
    sys.exit(main())
