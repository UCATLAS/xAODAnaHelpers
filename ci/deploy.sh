#!/bin/bash
docker login -u $DOCKERLOGIN -p $DOCKERPW
docker tag  ucatlas/xah:${ABV_CM}-latest ucatlas/xah:${ABV_CM}-${TRAVIS_COMMIT::6}-$(date +%Y%M%d)
docker push ucatlas/xah:${ABV_CM}-${TRAVIS_COMMIT::6}-$(date +%Y%M%d)
docker push ucatlas/xah:${ABV_CM}-latest
