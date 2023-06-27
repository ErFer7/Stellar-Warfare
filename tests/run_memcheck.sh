cd ..
make clean
make
cd bin
valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-limit=no --suppressions=./tests/SFML.supp --gen-suppressions=all --log-file=./tests/minimalraw.log "./Spaceship game"
cd ..
cat ./tests/minimalraw.log | ./tests/parse_valgrind_suppressions.sh > ./tests/minimal.supp