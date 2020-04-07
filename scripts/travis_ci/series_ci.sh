curl \
  --header "Authorization: Token 5e287d4c-a391-41e9-b2f5-8cfa38525052" \
  --header "Content-Type: application/json" \
  --data "{\"value\":\"8\",\"sha\":\"${TRAVIS_COMMIT}\"}" \
  https://seriesci.com/api/thekenu/Consolidated-Firmware/text/one

curl \
  --header "Authorization: Token 5e287d4c-a391-41e9-b2f5-8cfa38525052" \
  --header "Content-Type: application/json" \
  --data "{\"values\":[{\"value\":\"9\",\"line\":\"a\"}],\"sha\":\"${TRAVIS_COMMIT}\"}" \
  https://seriesci.com/api/thekenu/Consolidated-Firmware/data2/many

curl \
  --header "Authorization: Token 5e287d4c-a391-41e9-b2f5-8cfa38525052" \
  --header "Content-Type: application/json" \
  --data "{\"value\":\"10\",\"sha\":\"${TRAVIS_COMMIT}\"}" \
  https://seriesci.com/api/thekenu/Consolidated-Firmware/bss/one
