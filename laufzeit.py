import subprocess, time

pairs = [
    ("Praterstern", "Stephansplatz"),
    ("Praterstern", "Volkstheater"),
    ("Heiligenstadt", "Reumannplatz"),
    ("Huetteldorf", "Aspernstrasse"),
]

REPEATS = 20

for start, end in pairs:
    times = []
    for _ in range(REPEATS):
        t0 = time.perf_counter()
        
        subprocess.run(["./find_path", "./test_files/netz", start, end], capture_output=True)

        t1 = time.perf_counter()
        times.append((t1 - t0) * 1000)

    times.sort()
    print(f"{start} -> {end}: "
          f"min={times[0]:.2f} ms, "
          f"median={times[len(times)//2]:.2f} ms, "
          f"avg={sum(times)/len(times):.2f} ms")
