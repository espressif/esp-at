""" Handle PKI items for AT """

import os
import argparse


class LEUnsigned(object):
    """ convert for int and LE unsigned int bytes """
    @classmethod
    def pack(cls, le_unsigned_int, size):
        """ covert int to LE unisgned int bytes """
        output = bytes()
        for i in range(size):
            output += chr((le_unsigned_int >> (i*8)) & 0xFF)
        return output

    @classmethod
    def unpack(cls, bytes_in):
        """ convert LE unsigned int bytes to int """
        value = long()
        for i, _ in enumerate(bytes_in):
            value += ord(bytes_in[i]) << (i*8)
        return value


class PKIItem(object):
    """
    PKI item:

    | <- 1 byte -> | <- 1 byte -> | <- 2 bytes -> | <- content len -> | <- (4-len%4) % 4 bytes -> |
    +--------------+--------------+---------------+-------------------+---------------------------+
    |     type     |     ID       | content len   |    raw data       | padding for 4 bytes align |
    +--------------+--------------+---------------+-------------------+---------------------------+

    type: item type
        0x01: CA
        0x02: certificate
        0x03: key
    ID: used to match cert and key.
    content len: raw data length
    """

    TYPE = {
        "ca":   0x01,
        "cert": 0x02,
        "key":  0x03,
    }

    @classmethod
    def to_bytes(cls, pki_type, item_id, file_name):
        """ pack one pki item to bytes """
        with open(file_name, "rb") as _file:
            pki_raw_data = _file.read()

        data_out = LEUnsigned.pack(cls.TYPE[pki_type], 1)  # type
        data_out += LEUnsigned.pack(item_id, 1)  # ID
        data_out += LEUnsigned.pack(len(pki_raw_data), 2)  # content len
        data_out += pki_raw_data  # raw data
        data_out += chr(0xFF) * ((4 - (len(pki_raw_data) % 4)) % 4)  # padding
        return data_out

    @classmethod
    def output_file(cls, pki_type_enum, pki_id, raw_data, output_path):
        """ output one parsed pki item from bin to file """
        pki_type = None

        for pki_type in cls.TYPE:
            if cls.TYPE[pki_type] == pki_type_enum:
                break
        else:
            raise StandardError("pki type not supported: 0x%02x" % pki_type_enum)

        folder_path = os.path.join(output_path, pki_type)
        if os.path.exists(folder_path) is False:
            os.makedirs(folder_path)

        file_name = "%s_%d.%s" % (pki_type, pki_id, "key" if "key" in pki_type else "cer")

        with open(os.path.join(folder_path, file_name), "wb+") as _file:
            _file.write(raw_data)

    @classmethod
    def from_bytes(cls, bytes_in, output_path):
        """ parse one pki item to bin """

        pki_type = LEUnsigned.unpack(bytes_in[0])
        pki_id = LEUnsigned.unpack(bytes_in[1])
        content_len = LEUnsigned.unpack(bytes_in[2:4])
        raw_data = bytes_in[4:4+content_len]
        cls.output_file(pki_type, pki_id, raw_data, output_path)
        return bytes_in[4 + content_len + ((4 - (content_len % 4)) % 4):]


class FileFormat(object):
    """
    Converter of PKI data and files
    File Format ( unsigned int is little endian )

    | <- 2 bytes -> | <- 2 bytes -> | <- 4 bytes -> | <- total length -> |
    +---------------+---------------+---------------+--------------------+
    |   magic code  |  list size    |     length    |    list items      |
    +---------------+---------------+---------------+--------------------+

    magic code: 0xF1F1
    list size: count of total items
    length: total length of list items
    list items: a list of PKIItems
    """
    MAGIC_CODE = 0xF1F1

    @classmethod
    def separate_pki_items(cls, pki_list):
        """ sort pki items by type """

        pki_items_by_type = dict()
        for pki in pki_list:
            if pki["type"] in pki_items_by_type:
                pki_items_by_type[pki["type"]].append(pki["file"])
            else:
                pki_items_by_type[pki["type"]] = [pki["file"]]
        return pki_items_by_type

    @classmethod
    def to_bytes(cls, pki_list):
        """ pack pki items to files """

        assert isinstance(pki_list, list)
        assert pki_list

        pki_items_by_type = cls.separate_pki_items(pki_list)

        pki_items = list()

        for pki_type in pki_items_by_type:
            pki_list = pki_items_by_type[pki_type]
            pki_items.extend([PKIItem.to_bytes(pki_type, i, x) for i, x in enumerate(pki_list)])

        data = LEUnsigned.pack(cls.MAGIC_CODE, 2)  # magic code
        data += LEUnsigned.pack(len(pki_items), 2)  # list size
        data += LEUnsigned.pack(sum([len(x) for x in pki_items]), 4)  # length

        for pki in pki_items:
            data += pki

        return data

    @classmethod
    def from_bytes(cls, bytes_in, output_path):
        """ parse from bin file and save pki items to output path """

        magic_code = LEUnsigned.unpack(bytes_in[:2])
        list_size = LEUnsigned.unpack(bytes_in[2:4])
        length = LEUnsigned.unpack(bytes_in[4:8])

        assert magic_code == cls.MAGIC_CODE
        assert length == len(bytes_in) - 8

        actual_size = 0

        data = bytes_in[8:]

        force_break = 1000

        while not data and force_break > 0:
            data = PKIItem.from_bytes(data, output_path)
            force_break -= 1
            actual_size += 1

        assert actual_size == list_size


class PKIPairAction(argparse.Action):
    """ pki list parser """

    def __init__(self, option_strings, dest, nargs='+', **kwargs):
        super(PKIPairAction, self).__init__(option_strings, dest, nargs, **kwargs)

    def __call__(self, parser, namespace, values, option_string=None):
        pairs = []
        for i in range(0, len(values), 2):
            pairs.append({"type": values[i], "file": values[i+1]})
        setattr(namespace, self.dest, pairs)


def parse_args():
    """ parse arguments from command line """

    parser = argparse.ArgumentParser()

    subparsers = parser.add_subparsers(dest='operation',
                                       help='Run AtPKI {command} -h for additional help')

    parse_bin_parser = subparsers.add_parser("parse_bin",
                                             help="parse generated PKI bin")
    parse_bin_parser.add_argument("--bin_file", "-b", default="PKI.bin",
                                  help="bin_file which need to be parsed")
    parse_bin_parser.add_argument("--output_path", "-o",
                                  help="output path of parsed bin file in from_bytes mode")

    generate_bin_parser = subparsers.add_parser("generate_bin",
                                                help="create PKI bin")
    generate_bin_parser.add_argument("--bin_file", "-b", default="PKI.bin",
                                     help="bin_file which need to be parsed")
    generate_bin_parser.add_argument('pki_list', metavar='<type> <file>',
                                     help='type (ca, cert, key,)'
                                          ' and file, separated by space',
                                     action=PKIPairAction)

    args = parser.parse_args()
    return args


def main():
    """ main """

    args = parse_args()
    if args.operation == "generate_bin":
        data = FileFormat.to_bytes(args.pki_list)
        with open(args.bin_file, "wb") as _file:
            _file.write(data)
    elif args.operation == "parse_bin":
        with open(args.bin_file, "rb") as _file:
            data = _file.read()
        FileFormat.from_bytes(data, args.output_path)
    else:
        print "Not supported operation: %s" % args.operation
        exit(-1)


if __name__ == '__main__':
    main()
