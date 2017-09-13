# Environment configuration file setting up the installed xAODAnaHelpers project.

# Set up the base environment using the base image's setup script:
source ~/analysis_release_setup.sh

# Set up the xAODAnaHelpers installation:
source /usr/xAODAnaHelpers/*/InstallArea/*/setup.sh
echo "Configured xAODAnaHelpers from: ${xAODAnaHelpers_DIR}"

# Set up the prompt:
export PS1="\[\e[1;31;40m\]xAODAnaHelpers-$xAODAnaHelpers_VERSION\[\e[0m\]:\[\e[36;40m\]\w\[\e[1;33;40m\]$ \[\e[0m\]"
