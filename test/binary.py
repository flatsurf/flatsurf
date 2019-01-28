#*********************************************************************
#  This file is part of Polygon.
#
#        Copyright (C) 2019 Julian Rüth
#
#  Polygon is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  Polygon is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Polygon. If not, see <https://www.gnu.org/licenses/>.
#*********************************************************************

def make_time_vertices(cls):
    from .binary_data import expected_output
    for vertices in expected_output.keys():
        def time_vertices(self, vertices=vertices):
            self.run(vertices, expected_output[vertices].strip())
        import timeit
        time_vertices.timer = timeit.default_timer
        setattr(cls, "time_%s"%("_".join(map(str, vertices))), time_vertices)
    return cls

@make_time_vertices
class BinarySuite:
    r"""
    Tests the polygon binary by invoking it directly
    """
    def run(self, vertices, expected):
        from plumbum import local
        polygon = local["polygon"]
        output = polygon(*vertices,"--depth=10", "--follow_depth=50", "--show_cyls", "--show_moduli", "--cyls_only", "--quiet").strip()
        output = output[output.find('\n')+1:]
        if output != expected:
            from difflib import context_diff
            diff = [l.strip() for l in context_diff(expected.split('\n'), output.split('\n'), fromfile="expected", tofile="output")]
            raise Exception("Output did not match.\n" + "\n".join(diff))
