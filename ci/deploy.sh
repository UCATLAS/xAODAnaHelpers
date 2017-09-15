#!/bin/bash
docker login -u $DOCKERLOGIN -p $DOCKERPW
docker tag ucatlas/xah ucatlas/xah:$(date +%Y%M%d)-${ABV_CM}-${TRAVIS_COMMIT::6}
docker push ucatlas/xah:$(date +%Y%M%d)-${ABV_CM}-${TRAVIS_COMMIT::6}
