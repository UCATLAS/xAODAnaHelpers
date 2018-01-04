#!/bin/bash
docker login -u $DOCKERLOGIN -p $DOCKERPW
docker tag  ucatlas/xah:${DOCKER_TAG}-latest ucatlas/xah:${DOCKER_TAG}-${TRAVIS_COMMIT::6}-$(date +%Y%m%d)
docker push ucatlas/xah:${DOCKER_TAG}-${TRAVIS_COMMIT::6}-$(date +%Y%m%d)
docker push ucatlas/xah:${DOCKER_TAG}-latest
