# provide xAH on top of the AnalysisBase image
#ARG ABV_CM
FROM atlas/analysisbase:$ABV_CM

# Switch to ROOT user for now
USER root
# Copy the project's sources into the image
COPY . /workarea/src/xAODAnaHelpers
# this is needed to get rpmbuild temp dir in different place
COPY ci/.rpmmacros $HOME/.rpmmacros
# Build the project inside a build/ directory
RUN /workarea/src/xAODAnaHelpers/ci/compile.sh
RUN cd /workarea/build && cpack

# Install the created RPM
RUN rpm -i /workarea/build/*_*.rpm
# Clean up
RUN rm -rf /workarea

# Switch back to the ATLAS account
USER atlas
# Set up the environment setup script:
COPY ci/xAODAnaHelpers_setup.sh /home/atlas/xAODAnaHelpers_setup.sh
# Start the image with BASH by default
CMD /bin/bash
