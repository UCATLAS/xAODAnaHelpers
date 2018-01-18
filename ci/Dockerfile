# provide xAH on top of the AnalysisBase/AnalysisTop image
#ARG RELEASE
FROM atlas/$DOCKER_IMG

# change TMPDIR because analysisbase image problems writing to /tmp
ENV TMPDIR=/workarea/tmp/
ENV TRAVIS_COMMIT=${TRAVIS_COMMIT}
ENV TRAVIS_JOB_ID=${TRAVIS_JOB_ID}
WORKDIR $TMPDIR
WORKDIR /home/atlas

### COPY COMMANDS HERE ###

# Copy the project's sources into the image
COPY . /workarea/src/xAODAnaHelpers
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
RUN /workarea/src/xAODAnaHelpers/ci/compile.sh \
    && cd /workarea/build && cpack \
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
