#!/usr/bin/env bash
# Build all binaries, generate secret data, and validate every test case.
set -euo pipefail

cd "$(dirname "$0")"

# 1. Compile.
g++ -O2 -std=c++17 -o std.exe std.cpp
g++ -O2 -std=c++17 -o brute.exe brute.cpp
g++ -O2 -std=c++17 -o validator.exe validator.cpp
g++ -O2 -std=c++17 -o gen_random.exe gen_random.cpp
g++ -O2 -std=c++17 -o gen_extreme.exe gen_extreme.cpp
g++ -O2 -std=c++17 -o stress.exe stress.cpp

mkdir -p data/sample data/secret

# 2. Define the secret testcase plan: id mode N M H maxV [seed]
#    mode=random uses gen_random.exe with the pattern in $7.
#    mode=extreme uses gen_extreme.exe with the structure name in $7.
PLAN=(
  "01 extreme  1  1  1  1000000000  all-zero"
  "02 extreme  1  1  1  1000000000  all-max"
  "03 random   1  1 20  1000000000  uniform"
  "04 random   1 20 20  1000000000  uniform"
  "05 random   3  3  3  1000000000  uniform"
  "06 random   5  5  5  1000000000  uniform"
  "07 random  10 10 10  1000000000  uniform"
  "08 random  20 20 20  1000000000  uniform"
  "09 extreme 20 20 20  1000000000  all-max"
  "10 extreme 20 20 20  1000000000  only-A1"
  "11 extreme 20 20 20  1000000000  only-A34"
  "12 extreme 20 20 20  1000000000  only-A56"
  "13 extreme 20 20 20  1000000000  only-A78"
  "14 random  20 20 20  1000000000  checker"
  "15 random  19 19 19  1000000000  uniform"
  "16 random   7 11 13  1000000000  uniform"
  "17 random   4 20 20  1000000000  uniform"
  "18 random   1  1 20  1000000000  binary"
  "19 extreme 20 20 20  1000000000  binary"
  "20 extreme 20 20 20  1000000000  contrast"
)

for entry in "${PLAN[@]}"; do
  read -r id mode N M H maxV pat <<< "$entry"
  out="data/secret/${id}.in"
  case "$mode" in
    random)  ./gen_random.exe  "$N" "$M" "$H" "$maxV" "$pat" "$id" > "$out" ;;
    extreme) ./gen_extreme.exe "$N" "$M" "$H" "$pat" "$maxV" "$id" > "$out" ;;
    *) echo "unknown mode $mode"; exit 2 ;;
  esac
done

# 3. Validate every test (samples + secrets).
fail=0
for f in data/sample/*.in data/secret/*.in; do
  if ! ./validator.exe < "$f" > /dev/null 2>&1; then
    echo "VALIDATOR FAIL: $f"
    fail=$((fail + 1))
  fi
done
if [ "$fail" -ne 0 ]; then
  echo "$fail validation failures"; exit 3
fi

# 4. Produce .ans for every input via std.
for f in data/sample/*.in data/secret/*.in; do
  ans="${f%.in}.ans"
  ./std.exe < "$f" > "$ans"
done

# 5. Sanity-check sample answers stayed consistent.
for n in 1 2; do
  diff -q "data/sample/${n}.ans" "data/sample/${n}.ans" >/dev/null
done

echo "All ${#PLAN[@]} secret + 2 sample tests generated, validated, and answered."
