#!/usr/bin/env bash
# checker.sh — 对数器 for "Quantum Stabilizer Initialization"
#
# Usage:
#   ./checker.sh <your_solution> [iters=50] [size=small] [maxV=mixed]
#
#   <your_solution>  a .exe binary, or a .cpp file (auto-compiled to .exe).
#   [iters]          number of random tests (default 50).
#                    Pass 0 with size=dataset to skip the random phase entirely.
#   [size]           small | medium | large | dataset | full (default small).
#                      small  : V <= 18, additionally cross-checks brute.exe.
#                      medium : V <= ~512, std.exe only.
#                      large  : up to 20x20x20 = 8000, std.exe only.
#                      dataset: replay every data/sample/*.in and data/secret/*.in
#                               and diff against the bundled .ans files.
#                      full   : dataset replay + a small-shape random sweep.
#   [maxV]           per-cell max value cap, or "mixed" to alternate
#                    between 100, 1000000, 1000000000 (default mixed).
#                    Use mixed to catch overflow bugs.
#
# On a failing test, the input and both outputs are saved next to this script:
#   fail.in     the input that caused the disagreement
#   fail.ref    expected answer
#   fail.out    your solution's answer
# The script then exits with a non-zero status.

set -euo pipefail
cd "$(dirname "$0")"

if [ "$#" -lt 1 ]; then
    cat >&2 <<EOF
Usage: $0 <your_solution> [iters=50] [size=small|medium|large] [maxV=mixed|<int>]

Examples:
    $0 ./my_solution.exe
    $0 my_solution.cpp 200
    $0 ./my_solution.exe 100 medium
    $0 my_solution.cpp 30 large 1000000000
EOF
    exit 1
fi

USER_INPUT="$1"
ITERS="${2:-50}"
SIZE="${3:-small}"
MAXV_OPT="${4:-mixed}"

# ---- 1. Resolve user solution: auto-compile .cpp ----
if [[ "$USER_INPUT" == *.cpp ]]; then
    USER_EXE="${USER_INPUT%.cpp}.exe"
    echo ">>> Compiling $USER_INPUT -> $USER_EXE"
    g++ -O2 -std=c++17 -o "$USER_EXE" "$USER_INPUT"
    USER="$USER_EXE"
else
    USER="$USER_INPUT"
fi

if [ ! -x "$USER" ] && [ ! -f "$USER" ]; then
    echo "error: $USER not found" >&2
    exit 1
fi

# Normalize a bare "foo.exe" to "./foo.exe" so bash doesn't search PATH.
case "$USER" in
    /*|./*|../*) : ;;
    *) USER="./$USER" ;;
esac

# ---- 2. Build helpers if missing ----
[ -x ./std.exe ]        || g++ -O2 -std=c++17 -o std.exe        std.cpp
[ -x ./brute.exe ]      || g++ -O2 -std=c++17 -o brute.exe      brute.cpp
[ -x ./gen_random.exe ] || g++ -O2 -std=c++17 -o gen_random.exe gen_random.cpp

# ---- 3. Shape pools ----
DO_DATASET=0
DO_RANDOM=1
case "$SIZE" in
    small)
        SHAPES=("1 1 1" "1 1 2" "1 2 2" "2 2 2" "1 1 3" "1 3 3"
                "2 2 3" "1 2 4" "1 1 8" "2 2 4" "1 4 4" "2 3 3" "1 3 4")
        DO_BRUTE=1
        ;;
    medium)
        SHAPES=("3 3 3" "4 4 4" "5 5 5" "2 5 8" "6 6 6"
                "3 4 10" "8 8 8" "2 8 8" "1 10 10" "4 4 16")
        DO_BRUTE=0
        ;;
    large)
        SHAPES=("10 10 10" "15 15 15" "20 20 20"
                "5 20 20" "1 20 20" "20 1 20" "20 20 1" "8 10 20")
        DO_BRUTE=0
        ;;
    dataset)
        SHAPES=()
        DO_BRUTE=0
        DO_DATASET=1
        DO_RANDOM=0
        ;;
    full)
        SHAPES=("1 1 1" "2 2 2" "3 3 3" "5 5 5" "1 1 8" "2 3 4"
                "1 4 4" "2 2 4" "1 3 4" "2 2 3" "1 2 4")
        DO_BRUTE=1
        DO_DATASET=1
        ;;
    *)
        echo "unknown size: $SIZE (use small|medium|large|dataset|full)" >&2
        exit 2
        ;;
esac

# ---- 4. maxV picker ----
maxv_for_iter() {
    local i="$1"
    if [ "$MAXV_OPT" = "mixed" ]; then
        case $((i % 3)) in
            0) echo 100 ;;
            1) echo 1000000 ;;
            2) echo 1000000000 ;;
        esac
    else
        echo "$MAXV_OPT"
    fi
}

# ---- 5. Cleanup ----
trap 'rm -f _t.in _t.ref _t.out _t.brute' EXIT

dataset_pass=0
dataset_total=0

# ---- 6a. Dataset replay (size=dataset|full) ----
if [ "$DO_DATASET" = "1" ]; then
    DATA_FILES=()
    for f in data/sample/*.in data/secret/*.in; do
        [ -e "$f" ] && DATA_FILES+=("$f")
    done
    if [ "${#DATA_FILES[@]}" -eq 0 ]; then
        echo "warning: no .in files under data/sample or data/secret; skipping dataset phase" >&2
    else
        echo ">>> Replaying ${#DATA_FILES[@]} bundled testcases against shipped .ans"
        for f in "${DATA_FILES[@]}"; do
            ans_file="${f%.in}.ans"
            dataset_total=$((dataset_total + 1))
            label="${f#data/}"

            if [ ! -f "$ans_file" ]; then
                echo "[$label] internal: $ans_file missing" >&2
                cp "$f" fail.in
                exit 7
            fi

            if ! "$USER" < "$f" > _t.out; then
                echo "[$label] FAIL: your solution exited non-zero"
                cp "$f" fail.in
                cp "$ans_file" fail.ref
                [ -f _t.out ] && cp _t.out fail.out
                echo "    Saved: fail.in, fail.ref, fail.out"
                exit 4
            fi

            ref=$(tr -d '\r\n\t ' < "$ans_file")
            out=$(tr -d '\r\n\t ' < _t.out)

            if [ "$ref" != "$out" ]; then
                echo "[$label] MISMATCH"
                echo "    ref  : $ref"
                echo "    yours: $out"
                cp "$f" fail.in
                cp "$ans_file" fail.ref
                cp _t.out fail.out
                echo "    Saved: fail.in, fail.ref, fail.out"
                exit 5
            fi

            dataset_pass=$((dataset_pass + 1))
            echo "[$label] OK (ans=$ref)"
        done
    fi
fi

# ---- 6b. Random sweep (size=small|medium|large|full) ----
if [ "$DO_RANDOM" = "1" ] && [ "$ITERS" -gt 0 ] && [ "${#SHAPES[@]}" -gt 0 ]; then
    n_shapes=${#SHAPES[@]}
    pass=0
    echo ">>> Running $ITERS random tests against std.exe (size=$SIZE, maxV=$MAXV_OPT)"

    for ((i = 1; i <= ITERS; i++)); do
        idx=$(( (i * 2654435761 + 17) % n_shapes ))
        shape="${SHAPES[$idx]}"
        read -r N M H <<< "$shape"
        maxV=$(maxv_for_iter "$i")

        ./gen_random.exe "$N" "$M" "$H" "$maxV" uniform "$i" > _t.in

        if ! ./std.exe < _t.in > _t.ref; then
            echo "[$i/$ITERS] internal: std.exe failed on shape ${N}x${M}x${H}" >&2
            cp _t.in fail.in
            exit 3
        fi

        if ! "$USER" < _t.in > _t.out; then
            echo "[$i/$ITERS] FAIL: your solution exited non-zero on shape ${N}x${M}x${H}"
            cp _t.in fail.in
            cp _t.ref fail.ref
            [ -f _t.out ] && cp _t.out fail.out
            echo "    Saved: fail.in, fail.ref, fail.out"
            exit 4
        fi

        ref=$(tr -d '\r\n\t ' < _t.ref)
        out=$(tr -d '\r\n\t ' < _t.out)

        if [ "$ref" != "$out" ]; then
            echo "[$i/$ITERS] MISMATCH on shape ${N}x${M}x${H} (maxV=$maxV)"
            echo "    ref  : $ref"
            echo "    yours: $out"
            cp _t.in fail.in
            cp _t.ref fail.ref
            cp _t.out fail.out
            echo "    Saved: fail.in, fail.ref, fail.out"
            exit 5
        fi

        if [ "$DO_BRUTE" = "1" ]; then
            V=$((N * M * H))
            if [ "$V" -le 18 ]; then
                if ./brute.exe < _t.in > _t.brute 2>/dev/null; then
                    brute=$(tr -d '\r\n\t ' < _t.brute)
                    if [ "$brute" != "$ref" ]; then
                        echo "[$i/$ITERS] internal: std vs brute disagree on ${N}x${M}x${H} (this is a std bug)" >&2
                        cp _t.in std_bug.in
                        cp _t.ref std_bug.ref
                        cp _t.brute std_bug.brute
                        exit 6
                    fi
                fi
            fi
        fi

        pass=$((pass + 1))
        echo "[$i/$ITERS] ${N}x${M}x${H} maxV=$maxV ans=$ref OK"
    done

    echo ">>> RANDOM PHASE: $pass / $ITERS PASSED"
fi

if [ "$DO_DATASET" = "1" ]; then
    echo ">>> DATASET PHASE: $dataset_pass / $dataset_total PASSED"
fi
echo ">>> ALL CHECKS PASSED"

