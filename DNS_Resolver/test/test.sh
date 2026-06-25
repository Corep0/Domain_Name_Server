#!/bin/bash
server=8.8.8.8 # Google DNS by default
# read domains lines from the domain.txt file
while read domain
do
	echo "$domain"
	dns_result=$(./dns_resolver $domain | sort)
	dig_result=$(dig @$server +short $domain | sort)
	if echo "$dns_result" | grep -q "$dig_result"
	then
		echo "[OK] $domain : $dns_result"
	else
		echo "[FAIL] $domain"
	fi
done < ./test/domain.txt
