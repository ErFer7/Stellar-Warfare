cd ..
valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-limit=no --suppressions=./tests/SFML.supp --gen-suppressions=all --log-file=./tests/minimalraw.log ./bin/Spaceship\ game
cat ./tests/minimalraw.log | ./tests/parse_valgrind_suppressions.sh > ./tests/minimal.supp