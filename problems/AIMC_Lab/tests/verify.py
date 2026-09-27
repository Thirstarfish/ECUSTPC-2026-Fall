"""Compile and audit all 100 Polygon cases; build/data files live in a temp dir."""
import collections
import itertools
import pathlib
import re
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[1]


def brute(shape, values):
    coords = list(itertools.product(*(range(v) for v in shape)))
    k = len(coords)
    masks = [[sum(1 << j for j, q in enumerate(coords)
                  if sum((a-b)**2 for a, b in zip(p, q)) == d)
              for d in range(4)] for p in coords]
    best = 0
    full = (1 << k)-1
    for assignment in range(1 << k):
        total = 0
        for u in range(k):
            c = assignment >> u & 1
            for d in range(4):
                # Directly evaluate the original statement, without a flow graph.
                wanted = c ^ (d & 1)
                wrong = (full ^ assignment) if wanted else assignment
                if not masks[u][d] & wrong:
                    total += values[(2*d+c)*k+u]
        best = max(best, total)
    return best


def main():
    with tempfile.TemporaryDirectory(prefix='aimc-audit-') as directory:
        tmp = pathlib.Path(directory)
        def compile(source, name):
            subprocess.run(['g++', '-std=c++17', '-O2', '-Wall', '-Wextra',
                            '-I', str(ROOT), str(source), '-o', str(tmp/(name+'.exe'))], check=True)
        for name in ['generator', 'validator', 'script_generator']:
            compile(ROOT/(name+'.cpp'), name)
        # Measure phases in each ACTUAL reference implementation as well.
        for name, source, loop in [
            ('reference', 'solution_Thirstarfish.cpp', 'while (bfs(s, t))'),
            ('opus', 'solution_opus.cpp', 'while (bfs(s, t))')]:
            text = (ROOT/'solution'/source).read_text(encoding='utf-8')
            text = text.replace('using namespace std;', 'using namespace std;\nlong long audit_phases=0;')
            text = text.replace(loop, 'while (bfs(s, t) && (++audit_phases, true))')
            # A global destructor reports after main has produced its answer.
            text += '\nstruct Audit { ~Audit() { cerr << "phases=" << audit_phases << "\\n"; } } audit;\n'
            p = tmp/(name+'.cpp'); p.write_text(text, encoding='utf-8'); compile(p, name)
        def run(name, data='', args=(), ok=True):
            p = subprocess.run([str(tmp/(name+'.exe')), *map(str,args)], input=data,
                               text=True, capture_output=True, cwd=tmp, timeout=30)
            if ok: assert p.returncode == 0, (name, args, p.stderr)
            else: assert p.returncode != 0, (name, args)
            return p
        run('script_generator')
        script = (tmp/'script.txt').read_bytes()
        run('script_generator')
        assert script == (tmp/'script.txt').read_bytes() == (ROOT/'script.txt').read_bytes()
        lines = script.decode().splitlines()
        assert len(lines) == 100
        counts, metrics = collections.Counter(), collections.defaultdict(list)
        cases = []
        for index, line in enumerate(lines, 1):
            assert re.fullmatch(r'generator \d+ [1-9]\d* > \$', line)
            _, mode, seed, _, _ = line.split(); mode = int(mode)
            g = run('generator', args=(mode, seed)); data = g.stdout
            run('validator', data)
            numbers = list(map(int, data.split())); shape = numbers[:3]; values = numbers[3:]
            k = shape[0]*shape[1]*shape[2]
            assert len(values) == 8*k and 1 <= k <= 100
            assert all(0 <= v <= 10**9 for v in values)
            if 14 <= mode <= 21: assert k >= 90
            if mode >= 22: assert k == 100
            a, b = run('reference',data), run('opus',data)
            assert int(a.stdout) == int(b.stdout), index
            answer = int(a.stdout)
            if k <= 12: assert answer == brute(shape,values), index
            if mode in (4,6,8,10,12): assert answer == 4*k*10**9
            if mode in (7,21): assert answer == sum(values)
            if mode in (3,5): assert answer == 0
            if mode == 1: assert answer == 2265
            if mode == 2: assert answer == 88
            if mode >= 22:
                initial_p,initial_s,final_p,final_s = map(int,re.findall(r'=(\d+)',g.stderr))
                assert (final_p,final_s) >= (initial_p,initial_s) if mode in (22,24) else (final_s,final_p) >= (initial_s,initial_p)
            counts[mode] += 1
            metrics[mode].append((int(a.stderr.split('phases=')[-1]),int(b.stderr.split('phases=')[-1])))
            cases.append((mode,seed,data))
            if index%10 == 0: print(f'Checked {index}/100',flush=True)
        # Replay one seed from EVERY mode, including all search modes.
        for mode in range(1,26):
            _,seed,data = next(c for c in cases if c[0] == mode)
            assert run('generator',args=(mode,seed)).stdout == data
        for args in [(0,1),(26,1),(),('bad',1)]: run('generator',args=args,ok=False)
        sample = cases[0][2]
        for invalid in [sample+'extra\n',sample.replace('1 1 1','0 1 1',1),
                        sample.replace('1 1 1','2 3 17',1), sample.replace('\n3\n','\n-1\n',1),
                        sample.replace('\n3\n','\n1000000001\n',1),sample.replace('\n\n','\n',1)]:
            run('validator',invalid,ok=False)
        for mode in range(14,26):
            records=metrics[mode]
            print(f'mode {mode}: reference phases {min(r[0] for r in records)}..{max(r[0] for r in records)}; '
                  f'opus phases {min(r[1] for r in records)}..{max(r[1] for r in records)}')
        print('PASS: all 100 cases, brute-force tiny cases, exact boundaries, replay and rejection tests.')


if __name__ == '__main__': main()
