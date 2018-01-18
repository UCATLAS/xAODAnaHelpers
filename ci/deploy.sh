#!/bin/bash
docker login -u $DOCKERLOGIN -p $DOCKERPW
docker tag  ${DOCKER_ORG}/${DOCKER_REPO}:${DOCKER_TAG}-latest ${DOCKER_ORG}/${DOCKER_REPO}:${DOCKER_TAG}-${TRAVIS_COMMIT::6}-$(date +%Y%m%d)
docker push ${DOCKER_ORG}/${DOCKER_REPO}:${DOCKER_TAG}-${TRAVIS_COMMIT::6}-$(date +%Y%m%d)
docker push ${DOCKER_ORG}/${DOCKER_REPO}:${DOCKER_TAG}-latest
