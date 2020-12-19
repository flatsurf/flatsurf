# -*- coding: utf-8 -*-
#*********************************************************************
#  This file is part of flatsurf.
#
#        Copyright (C) 2019-2020 Julian Rüth
#
#  Flatsurf is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  Flatsurf is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
#*********************************************************************

import os
from os.path import join

from .cppasv import create_wrappers

ASV_PROJECT_DIR = os.environ.get('ASV_PROJECT_DIR', None)

if not ASV_PROJECT_DIR:
    ASV_ENV_DIR = os.environ.get('ASV_ENV_DIR', None)
    if ASV_ENV_DIR:
        ASV_PROJECT_DIR = join(ASV_ENV_DIR, "project")
    else:
        ASV_PROJECT_DIR = join(os.path.dirname(os.path.abspath(__file__)), "..", "..")

locals().update(create_wrappers(join(ASV_PROJECT_DIR, "libflatsurf", "benchmark", "benchmark")))
