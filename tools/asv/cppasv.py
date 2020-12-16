# -*- coding: utf-8 -*-
#*********************************************************************
#  This file is part of cppasv.
#
#        Copyright (C) 2019-2020 Julian Rüth
#
#  cppasv is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  cppasv is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with cppasv. If not, see <https://www.gnu.org/licenses/>.
#*********************************************************************

# This module turns google (C++) benchmarks into Python classes that can be run
# by AirSpeedVelocity which provides a nice web interface to see how benchmarks
# evolve over time.

# If you want to use this module, make sure NOT to add the conda-forge package
# 'benchmark' (google benchmark) to your asv.conf.json matrix as that is a
# reserved key in ASV. Instead add an exact pin such as 'benchmark==1.4.1'.

import subprocess

def create_wrappers(benchmark):
    r"""
    Return ASV compatible benchmark classes that contain a `track_time` method
    for each benchmark exported by the Google Benchmark binary `benchmark`.
    """
    benchmarks = subprocess.check_output([benchmark, "--benchmark_list_tests"]).decode('UTF-8').split('\n')

    benchmarks = [benchmark.strip().split('/') for benchmark in benchmarks if benchmark.strip()]

    benchmarks = create_time_methods(benchmark, benchmarks)

    benchmarks = { sanitize_benchmark_name(benchmark): benchmarks[benchmark] for benchmark in benchmarks }

    return {benchmark: create_benchmark_class(benchmark, time_method) for (benchmark, time_method) in benchmarks.items()}

def create_benchmark_class(name, time_method):
    r"""
    Return an ASV compatible benchmark class called `name` that runs the given method as `track_time`.
    """
    class Benchmark:
        track_time = time_method
    Benchmark.__name__ = name

    return Benchmark

def sanitize_benchmark_name(name):
    r"""
    Return the Google Benchmark name `name` rewritten as a valid Python 3 identifier.
    """
    name = name.strip()

    # < and > are not allowed in Python identifiers, so we replace them with similarly looking unicode characters.
    name = name.replace("<", "⟨")
    name = name.replace(">", "⟩")

    return name

def create_time_methods(benchmark, benchmarks):
    r"""
    Return a mapping from sanitized benchmark names to Python methods that run that benchmark.
    """
    toplevel = set(b[0] for b in benchmarks)

    time_methods = { name: create_time_method(benchmark, name) for name in toplevel }
    
    params = { name: [tuple(b[1:]) for b in benchmarks if b[0] == name] for name in toplevel }

    for name in toplevel:
        if len(params[name]) > 1:
            time_methods[name].params = [ ", ".join(p) for p in params[name] ]

    return time_methods

def create_time_method(benchmark, name):
    r"""
    Return a method that runs the benchmark binary `benchmark` on the Google Benchmark called `name`.
    """
    def run_benchmark(self, params=None):
        filter = f"{name}"
        if params:
            filter = f"{name}/{params.replace(', ', '/')}"
        out = subprocess.check_output([benchmark, f"--benchmark_filter={filter}", "--benchmark_format=json"])

        import json
        out = json.loads(out)

        assert(len(out["benchmarks"]) == 1)
        time = out["benchmarks"][0]["cpu_time"]
        unit = out["benchmarks"][0]["time_unit"]

        if unit == "s":
            time *= 1000
            unit = "ms"
        if unit == "ms":
            time *= 1000
            unit = "us"
        if unit == "us":
            time *= 1000
            unit = "ns"

        assert(unit == "ns")

        return time

    run_benchmark.unit = "ns"

    # We cannot compute a version number of this benchmark yet since we cannot hash its original C++ source code easily.
    run_benchmark.version = 0

    # Google Benchmark already takes care of sampling for us, so we only run the benchmarks once.
    run_benchmark.repeat = 1
    run_benchmark.number = 1
    run_benchmark.min_run_count = 1

    # Show the original benchmark name in ASV output
    # Unfortunately, this breaks ASV's HTML output.
    # run_benchmark.name = name
    # run_benchmark.pretty_name = name

    return run_benchmark
