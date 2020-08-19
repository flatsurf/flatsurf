######################################################################
#  This file is part of flatsurf.
#
#        Copyright (C) 2020 Julian Rüth
#
#  pyeantic is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or (at your
#  option) any later version.
#
#  pyeantic is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with pyeantic. If not, see <https://www.gnu.org/licenses/>.
#####################################################################

import sys
sys.path.insert(0, 'recipe/snippets/rever')

import dist

$PROJECT = 'flatsurf'

$ACTIVITIES = [
    'version_bump',
    'changelog',
    'dist',
    'tag',
    'push_tag',
    'ghrelease',
]

$VERSION_BUMP_PATTERNS = [
    ('configure.ac', r'AC_INIT', r'AC_INIT([flatsurf], [$VERSION], [julian.rueth@fsfe.org])'),
    ('libflatsurf/configure.ac', r'AC_INIT', r'AC_INIT([libflatsurf], [$VERSION], [julian.rueth@fsfe.org])'),
    ('pyflatsurf/configure.ac', r'AC_INIT', r'AC_INIT([pyflatsurf], [$VERSION], [julian.rueth@fsfe.org])'),
    ('recipe/meta.yaml', r"\{% set version =", r"{% set version = '$VERSION' %}"),
]

print("Make sure you bump the version_info in libflatsurf/configure.ac manually!")

$CHANGELOG_FILENAME = 'ChangeLog'
$CHANGELOG_TEMPLATE = 'TEMPLATE.rst'
$PUSH_TAG_REMOTE = 'git@github.com:flatsurf/flatsurf.git'

$GITHUB_ORG = 'flatsurf'
$GITHUB_REPO = 'flatsurf'

$GHRELEASE_ASSETS = ['flatsurf-' + $VERSION + '.tar.gz']
