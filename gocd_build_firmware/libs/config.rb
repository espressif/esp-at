# Config taken from environment variables.
# This is used in the Vagrantfile as well as the Rake scripts.

def run_by_gocd()
  return ENV.has_key?('GO_PIPELINE_NAME')
end

