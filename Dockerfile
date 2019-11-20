# provide xAH on top of the AnalysisBase/AnalysisTop image
ARG DOCKER_REPO=atlas
ARG DOCKER_IMG=analysisbase
ARG DOCKER_TAG=latest
ARG GIT_SHA=private
FROM $DOCKER_REPO/$DOCKER_IMG:$DOCKER_TAG

# change TMPDIR because analysisbase image problems writing to /tmp
ENV TMPDIR=/workarea/tmp/
ENV GIT_SHA=$GIT_SHA
ENV RELEASE_TYPE=$DOCKER_IMG
WORKDIR $TMPDIR
WORKDIR /home/atlas

### COPY COMMANDS HERE ###

# Copy the project's sources into the image
COPY . /workarea/src/xAODAnaHelpers
COPY ci/top_CMakeLists.txt /workarea/src/CMakeLists.txt.tmp
# this is needed to get rpmbuild temp dir in different place
COPY ci/.rpmmacros /root/.rpmmacros
# Use our MOTD (Message-of-the-Day)
COPY ci/motd /etc/motd
# Copy the environemnt setup script for xAH
COPY ci/xAODAnaHelpers_setup.sh /home/atlas/xAODAnaHelpers_setup.sh

### RUN COMMANDS HERE (AS ROOT) ###
# Switch to ROOT user for now
USER root
# 1. Build the project inside a build/ directory
# 2. Create an RPM
# 3. Install the RPM
# 4. Clean up
# 5. Call the MOTD
# 6. Call the environment setup script in .bashrc
RUN echo $RELEASE_TYPE \
    && echo $([ "$RELEASE_TYPE" == "analysisbase" ] && echo "AnalysisBase" || echo "AnalysisTop") \
    && export RELEASE_TYPE=$([ "$RELEASE_TYPE" == "analysisbase" ] && echo "AnalysisBase" || echo "AnalysisTop") \
    && echo $RELEASE_TYPE \
    && envsubst '\$RELEASE_TYPE' < /workarea/src/CMakeLists.txt.tmp > /workarea/src/CMakeLists.txt \
    && source /home/atlas/release_setup.sh \
    && mkdir -p /workarea/build \
    && cd /workarea/build \
    && time cmake ../src \
    && time make \
    && cpack \
    && rpm -i /workarea/build/*_*.rpm \
    && rm -rf /workarea \
    && echo '[ ! -z "$TERM" -a -r /etc/motd ] && cat /etc/motd' >> /home/atlas/.bashrc \
    && echo 'source $HOME/xAODAnaHelpers_setup.sh' >> /home/atlas/.bashrc

# set TMPDIR back
ENV TMPDIR=/tmp
# Switch back to the ATLAS account
USER atlas
# Start the image with BASH by default
CMD /bin/bash

### TO DO ENTRYPOINT
#RUN echo -e '#!/bin/bash\nsource ~/.bashrc\nxAH_run.py "$@"' > $HOME/entrypoint.sh
#RUN chmod +x $HOME/entrypoint.sh
#ENTRYPOINT ["/home/atlas/entrypoint.sh"]
