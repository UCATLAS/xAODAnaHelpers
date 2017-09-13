# provide xAH on top of the AnalysisBase image
ARG ABV_CM

FROM atlas/analysisbase:$ABV_CM

# Copy the project's sources into the image
COPY . /workarea/src/xAODAnaHelpers
# Build the project inside a build/ directory
RUN /workarea/src/xAODAnaHelpers/ci/run.sh
RUN cd /workarea/build && cpack

# Install the created RPM as root
USER root
RUN rpm -i /workarea/build/*_*.rpm

# Clean up
RUN rm -rf /workarea/source /workarea/build

# Switch back to the ATLAS account
USER atlas

# Set up the environment setup script:
RUN mv release_setup.sh analysis_release_setup.sh
COPY ci/docker_analysis_setup.sh /home/atlas/setup.sh

# Start the image with BASH by default
CMD /bin/bash
