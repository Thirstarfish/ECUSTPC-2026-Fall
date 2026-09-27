"""Run with python problems/Light_Reflection/tests/checker_tests.py."""
import itertools
import pathlib
import random
import re
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[1]


def trace(grid, start):
    n, m = len(grid), len(grid[0])
    side, p = start
    r, c, d = {'U': (0, p-1, 2), 'D': (n-1, p-1, 0),
               'L': (p-1, 0, 1), 'R': (p-1, m-1, 3)}[side]
    seen, count = set(), 0
    while 0 <= r < n and 0 <= c < m:
        if (r, c, d) in seen:
            return None, -1
        seen.add((r, c, d))
        count += 1
        d ^= {'.': 0, '/': 1, '\\': 3}[grid[r][c]]
        r += (-1, 0, 1, 0)[d]
        c += (0, 1, 0, -1)[d]
    end = ('U', c+1) if r < 0 else ('D', c+1) if r >= n else ('L', r+1) if c < 0 else ('R', r+1)
    return end, count


def encode(cases):
    return str(len(cases)) + '\n' + ''.join(
        f'{len(g)} {len(g[0])}\n' + '\n'.join(g) + '\n' +
        f'{s[0]} {s[1]} {t[0]} {t[1]}\n' for g, s, t in cases)


def brute(case):
    grid, start, target = case
    free = [(r, c) for r, row in enumerate(grid) for c, ch in enumerate(row) if ch == '.']
    best, witness = 10**9, 'IMPOSSIBLE\n'
    for values in itertools.product('./\\', repeat=len(free)):
        g = list(map(list, grid))
        for (r, c), ch in zip(free, values):
            g[r][c] = ch
        end, count = trace(g, start)
        if end == target and count < best:
            best, witness = count, '\n'.join(map(''.join, g)) + '\n'
    return witness


def main():
    with tempfile.TemporaryDirectory(prefix='light-checker-') as tmp:
        tmp = pathlib.Path(tmp)
        programs = {'checker': ROOT/'checker.cpp', 'generator': ROOT/'generator.cpp',
                    'validator': ROOT/'validator.cpp',
                    'ports': ROOT/'solution/soluton_Thirstarfish.cpp',
                    'directions': ROOT/'solution/solution_Gemini.cpp'}
        for name, source in programs.items():
            subprocess.run(['g++', '-std=c++17', '-O2', '-Wall', '-Wextra', str(source), '-o', str(tmp/f'{name}.exe')], check=True)

        def run(name, data='', args=()):
            return subprocess.run([str(tmp/f'{name}.exe'), *map(str, args)], input=data,
                                  text=True, capture_output=True, timeout=30)

        def check(data, participant, jury, expected=0):
            for name, content in [('input', data), ('output', participant), ('answer', jury)]:
                (tmp/name).write_text(content, encoding='utf-8')
            result = run('checker', args=[tmp/'input', tmp/'output', tmp/'answer'])
            assert result.returncode == expected, (expected, result.returncode, result.stderr)

        for folder in ['statements_2026_fall', 'statements_2026_fall_en']:
            statement = (ROOT.parents[1]/folder/'Problem_H_Light_Reflection.tex').read_text(encoding='utf-8')
            blocks = re.findall(r'\\begin\{lstlisting\}[^\n]*\n(.*?)\\end\{lstlisting\}', statement, re.S)
            data, jury = blocks
            check(data, jury, jury)

        base = encode([(['..', '..'], ('L', 1), ('R', 1))])
        optimal, longer = '..\n..\n', '\\/\n\\/\n'
        check(base, optimal, optimal)
        check(base, longer, optimal, 1)
        check(base, optimal, longer, 3)
        check(base, 'IMPOSSIBLE\n', optimal, 1)
        check(base, optimal, 'IMPOSSIBLE\n', 3)
        check(base, 'IMPOSSIBLE\n', 'IMPOSSIBLE\n', 3)
        for bad in ['.\n..\n', 'xx\n..\n', '//\n..\n', optimal+'extra\n']:
            check(base, bad, optimal, 1)
            check(base, optimal, bad, 3)
        fixed = encode([(['/'], ('L', 1), ('U', 1))])
        check(fixed, '.\n', '/\n', 1)
        check(fixed, '/\n', '.\n', 3)
        impossible = encode([(['/'], ('L', 1), ('R', 1))])
        check(impossible, 'IMPOSSIBLE\n', 'IMPOSSIBLE\n')
        # The fixed bottom-right mirror must be visited twice (5 visits, 4 distinct cells).
        repeated = (['..', '.\\'], ('D', 2), ('R', 2))
        assert trace(brute(repeated).splitlines(), repeated[1])[1] == 5

        rng = random.Random(20260927)
        cases = [repeated]
        for n, m in [(1, 1), (1, 2), (2, 1), (2, 2)]:
            endpoints = [(s, p) for s in 'UDLR' for p in range(1, (m if s in 'UD' else n)+1)]
            for chars in itertools.product('./\\', repeat=n*m):
                grid = [''.join(chars[r*m:(r+1)*m]) for r in range(n)]
                for start, target in itertools.permutations(endpoints, 2):
                    cases.append((grid, start, target))
        for _ in range(100):
            grid = [''.join(rng.choices('./\\', k=3)) for _ in range(3)]
            ends = [(s, p) for s in 'UDLR' for p in range(1, 4)]
            start, target = rng.sample(ends, 2)
            cases.append((grid, start, target))
        for offset in range(0, len(cases), 100):
            batch = cases[offset:offset+100]
            data, jury = encode(batch), ''.join(map(brute, batch))
            for solver in ['ports', 'directions']:
                result = run(solver, data)
                assert result.returncode == 0, result.stderr
                check(data, result.stdout, jury)
        print(f'Passed {len(cases)} exhaustive/random oracle cases for both solutions and checker verdict tests.')

        for line in (ROOT/'script.txt').read_text().splitlines():
            tokens = line.split()
            if not tokens:
                continue
            generated = run('generator', args=tokens[1:tokens.index('>')])
            assert generated.returncode == 0, generated.stderr
            data = generated.stdout
            validated = run('validator', data)
            assert validated.returncode == 0, validated.stderr
            jury, participant = run('ports', data), run('directions', data)
            assert jury.returncode == participant.returncode == 0
            check(data, participant.stdout, jury.stdout)
        print('Passed every generator command in script.txt, including maximum-size cases.')


if __name__ == '__main__':
    main()
