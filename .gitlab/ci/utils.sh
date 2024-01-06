# copy from esp-idf/tools/ci/utils.sh

function add_ssh_keys() {
  local key_string="${1}"
  mkdir -p ~/.ssh
  chmod 700 ~/.ssh
  echo -n "${key_string}" >~/.ssh/id_rsa_base64
  base64 --decode --ignore-garbage ~/.ssh/id_rsa_base64 >~/.ssh/id_rsa
  chmod 600 ~/.ssh/id_rsa
}

function add_gitlab_ssh_keys() {
  add_ssh_keys "${GITLAB_KEY}"
  echo -e "Host gitlab.espressif.cn\n\tStrictHostKeyChecking no\n" >>~/.ssh/config

  # For gitlab geo nodes
  if [ "${LOCAL_GITLAB_SSH_SERVER:-}" ]; then
    SRV=${LOCAL_GITLAB_SSH_SERVER##*@} # remove the chars before @, which is the account
    SRV=${SRV%%:*}                     # remove the chars after :, which is the port
    printf "Host %s\n\tStrictHostKeyChecking no\n" "${SRV}" >>~/.ssh/config
  fi
}

function add_github_ssh_keys() {
  add_ssh_keys "${GH_PUSH_KEY}"
  echo -e "Host github.com\n\tStrictHostKeyChecking no\n" >>~/.ssh/config
}

function add_doc_server_ssh_keys() {
  local key_string="${1}"
  local server_url="${2}"
  local server_user="${3}"
  add_ssh_keys "${key_string}"
  echo -e "Host ${server_url}\n\tStrictHostKeyChecking no\n\tUser ${server_user}\n" >>~/.ssh/config
}

function get_module_cfg_dir() {
  module_name_lower=$(echo "${MODULE_NAME}" | tr '[:upper:]' '[:lower:]')
  module_cfg_dir="${CI_PROJECT_DIR}/module_config/module_${module_name_lower}"

  if [ ! -d "${module_cfg_dir}" ]; then
      platform_name_str=$(echo "${PLATFORM}" | sed 's/PLATFORM_//')
      module_name_lower=$(echo "${platform_name_str}" | tr '[:upper:]' '[:lower:]')
      module_cfg_dir="module_${module_name_lower}_default"
  else
      module_cfg_dir="module_${module_name_lower}"
  fi

  echo "current configuration dir: ${module_cfg_dir}"
}
