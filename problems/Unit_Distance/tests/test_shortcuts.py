"""Compare elementary grids, audit every n, then run the existing Polygon script."""
import math
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def edges(n, w, directions):
    h, r = divmod(n, w)
    result = 0
    for dx, dy in directions:
        if dy == 0:
            result += h*max(0, w-dx)+max(0, r-dx)
        else:
            result += max(0, h-dy)*max(0, w-abs(dx))
            if r and h >= dy:
                result += max(0, min(w, r-dx)-max(0, -dx))
    return result


def main():
    req = [0]+[max(0, ((n**n).bit_length()-1)//2-n) for n in range(1, 5001)]
    square = [(1, 0), (0, 1)]
    triangle = square+[(-1, 1)]
    fifth = [(5, 0), (0, 5), (3, 4), (-3, 4), (4, 3), (-4, 3)]
    for mode in ['line', 'square', 'triangle', 'fifth', 'hybrid']:
        bad = []
        for n in range(2, 5001):
            w = math.isqrt(n-1)+1
            directions = square if mode == 'square' else triangle if mode == 'triangle' or (mode == 'hybrid' and n < 100) else fifth
            count = n-1 if mode == 'line' else edges(n, w, directions)
            if count < req[n]:
                bad.append(n)
        print(f'{mode}: first failing n={bad[0] if bad else None}, failures={len(bad)}, pairs at 5000={count}', flush=True)
    with tempfile.TemporaryDirectory(prefix='unit-shortcuts-') as temp:
        work = Path(temp)
        targets = {
            'checker': ROOT/'checker.cpp',
            'generator': ROOT/'generator.cpp',
            'solution': ROOT/'tests/shortcut_grid.cpp',
            'audit': ROOT/'tests/audit_shortcut.cpp',
        }
        for name, source in targets.items():
            subprocess.run(['g++', '-std=c++17', '-O2', str(source), '-o', str(work/(name+'.exe'))], check=True)
        requirements = work/'requirements.txt'
        requirements.write_text('\n'.join(map(str, req[1:])))
        subprocess.run([str(work/'audit.exe'), str(requirements)], check=True)
        passed = 0
        for line in (ROOT/'script.txt').read_text().splitlines():
            if not line.strip():
                continue
            args = line.split()[1:3]
            generated = subprocess.run([str(work/'generator.exe'), *args], capture_output=True, text=True, check=True)
            solution = subprocess.run([str(work/'solution.exe')], input=generated.stdout, capture_output=True, text=True, check=True)
            inp, out = work/'case.in', work/'case.out'
            inp.write_text(generated.stdout)
            out.write_text(solution.stdout)
            check = subprocess.run([str(work/'checker.exe'), str(inp), str(out), str(out)], capture_output=True, text=True)
            if check.returncode:
                raise RuntimeError(f'{line}: {check.stderr}')
            passed += 1
        print(f'Existing Polygon generator script: {passed}/{passed} passed')


if __name__ == '__main__':
    main()
