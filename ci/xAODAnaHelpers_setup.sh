#!/bin/bash

# Environment configuration file setting up the installed xAODAnaHelpers project.

# Set up the base environment using the base image's setup script:
source /release_setup.sh

# Set up the xAODAnaHelpers installation:
source /usr/xAODAnaHelpers/*/InstallArea/*/setup.sh
echo "Configured xAODAnaHelpers from: ${xAODAnaHelpers_DIR}"

# Set up the prompt:
export PS1="\[\033[0;35m\][\u]\[\e[0;31m\][xAH-${xAODAnaHelpers_VERSION::6}]\[\e[0;35m\]:\[\e[0;36m\]\w\[\e[1;33m\]$ \[\e[0m\]"
