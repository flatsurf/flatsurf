/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2018 Alex Eskin
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Flatsurf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include <getopt.h>
#include <unistd.h>
#include <boost/numeric/conversion/cast.hpp>
#include <exact-real/number_field_traits.hpp>
#include <fstream>
#include <iostream>

#include "flatsurf/ccw.hpp"
#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/orientation.hpp"
#include "flatsurf/saddle_connections.hpp"
#include "flatsurf/vector_arb.hpp"
#include "flatsurf/vector_exactreal.hpp"

#include "./globals.h"
#include "./my_ostream.h"
#include "./number_field.h"
#include "./shared.h"
#include "./two_complex.h"
#include "./uedge.h"

using namespace polygon;

using boost::numeric_cast;
using exactreal::NumberFieldTraits;
using flatsurf::Bound;
using flatsurf::FlatTriangulation;
using flatsurf::HalfEdge;
using flatsurf::SaddleConnections;
using flatsurf::VectorExactReal;
using std::cout;
using std::endl;
using std::list;
using std::max;
using std::min;
using std::ofstream;

int main(int argc, char** argv) {
  bool file_arg = false;

  S = new TwoComplex();

  COORD t1, t2;

#ifdef USE_MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  //    MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
  my_mpi_init();
#endif

#ifdef USE_PVM
  NA.spawn(argc, argv);
#endif

#define MAX_SLICES 1
  out_f = stdout;
  n_slices = 1;

  for (int i = 0; i < argc; i++) {
    std::cout << argv[i] << " ";
  }
  std::cout << endl;

  while (1) {
    //	int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
        {"delta", required_argument, nullptr, 'a'},
        {"retriangulate", no_argument, nullptr, 'b'},
        {"move_vertex", required_argument, nullptr, 'c'},
        {"depth", required_argument, nullptr, 'd'},
        {"file", required_argument, nullptr, 'f'},

        {"lock", no_argument, nullptr, 'l'},
        {"max_priority", required_argument, nullptr, 'm'},
        {"offset", required_argument, nullptr, 'o'},
        {"perturb", optional_argument, nullptr, 'p'},
        {"quiet", no_argument, nullptr, 'q'},
        {"randomize", optional_argument, nullptr, 'r'},
        {"show_moduli", no_argument, nullptr, 't'},
        {"cyls_only", no_argument, nullptr, 'u'},
        {"verbose", optional_argument, nullptr, 'v'},
        {"perturb_conjugate", optional_argument, nullptr, 'x'},

        {"follow_depth", required_argument, nullptr, 'z'},
        {"no_field_arithmetic", no_argument, nullptr, 'A'},
        {"no_retriang", no_argument, nullptr, 'B'},
        {"closure", no_argument, nullptr, 'C'},
        {"allow_long_cyls", no_argument, nullptr, 'D'},
        {"epsilon", required_argument, nullptr, 'E'},
        {"show_length_list", no_argument, nullptr, 'F'},
        {"draw_saddles", required_argument, nullptr, 'G'},
        {"draw_cylinders", required_argument, nullptr, 'H'},

        {"tag", required_argument, nullptr, 'J'},
        {"billiard_mode", no_argument, nullptr, 'K'},
        {"show_lengths", no_argument, nullptr, 'L'},

        {"nice", required_argument, nullptr, 'N'},
        {"tikz_output", no_argument, nullptr, 'P'},

        {"norescale", no_argument, nullptr, 'R'},
        {"slices", required_argument, nullptr, 'S'},

        {"libflatsurf", no_argument, nullptr, '%'},
        {"show_cyls", no_argument, nullptr, 'Y'},
        {"heights_and_twists", no_argument, nullptr, 'Z'},
        {nullptr, 0, nullptr, 0}};

    int c;
    c = getopt_long(argc, argv, "f:s:e:d:v::lp::o:ti", long_options,
                    &option_index);
    if (c == -1) break;

    switch (c) {
      case 'd':
        if (optarg) {
          depth = atof(optarg);
        } else
          ERR_RET("depth: no arg");
        break;

      case 'z':
        if (optarg) {
          follow_depth = atof(optarg);
        } else
          ERR_RET("follow_depth: no arg");
        break;

      case 'v':
        verbose = 1;
        if (optarg) {
          verbose = atoi(optarg);
        }
        break;

      case 'l':
        perturb_euclidean = false;
        break;

      case 'r':
        if (optarg) {
          random_engine = std::default_random_engine{
              static_cast<unsigned int>(atoll(optarg))};
        } else {
          random_engine = std::default_random_engine{
              static_cast<unsigned int>(time(nullptr))};
        }
        break;

      case 'q':
        quiet = true;
        break;

      case 'B':
        retriangulate = false;
        break;

      case 'R':
        norescale = true;
        break;

      case 'C':
        closure = true;
        break;

      case 'A':
        no_field_arithmetic = true;
        break;

      case 'K':
        billiard_mode = true;
        retriangulate = false;
        perturb_euclidean = false;
        break;

      case 'P':
        tikz_output = true;
        break;

      case 'p':
        perturb_all = true;
        if (optarg) {
          perturb_magnitude = atof(optarg);
        }
        break;

      case 'x':
        perturb_conjugate = true;
        if (optarg) {
          perturb_magnitude = atof(optarg);
        }
        break;

      case 'c':
        if (optarg) {
          perturb_vertex = atoi(optarg);
          perturb_all = false;
        } else
          ERR_RET("move_vertex: no arg");
        break;

      case '%':
        libflatsurf = true;
        break;

      case 'o':
        if (optarg) {
#ifdef USE_LONG_DOUBLE
          if (sscanf(optarg, "%Lf,%Lf", &t1, &t2) == 2) {
#else
          if (sscanf(optarg, "%lf,%lf", &t1, &t2) == 2) {
#endif
            offset = Point(t1, t2);
          } else
            ERR_RET("offset: bad args");
        } else
          ERR_RET("offset: no arg");

        break;

      case 'a':
        if (optarg) {
          DELTA = atof(optarg);
        } else
          ERR_RET("delta: bad arg");

        break;

      case 'E':
        if (optarg) {
          EPSILON = atof(optarg);
          cout << "epsilon= " << EPSILON << endl;
        } else
          ERR_RET("epsilon: bad arg");

        break;

      case 'G':
        if (optarg) {
          draw_saddle_length = atof(optarg);
          draw_saddles = true;
        } else
          ERR_RET("draw_saddles: bad arg");

        break;

      case 'H':
        if (optarg) {
          draw_saddle_length = atof(optarg);
          draw_cylinders = true;
          show_cyls = true;
          show_moduli = true;
        } else
          ERR_RET("draw_cylinders: bad arg");

        break;

      case 'J':
        if (optarg) {
          draw_tag = atoi(optarg);
        } else
          ERR_RET("tag: bad arg");

        break;

      case 'f':
        if (optarg) {
          strncpy(filename_, optarg, 999);
          filename_[999] = '\0';
          file_arg = true;
        } else {
          ERR_RET("file: bad arg");
        }

        break;

      case 'm':
        if (optarg) {
          max_priority = atoi(optarg);
        } else
          ERR_RET("max_priority: no arg");
        break;

      case 'N':
        if (optarg) {
          nice_inc = atoi(optarg);
        } else
          ERR_RET("nice: no arg");
        break;

      case 'S':
        if (optarg) {
          n_slices = atoi(optarg);
        } else
          ERR_RET("slices: no arg");
        break;

      case 'L':
        show_lengths = true;
        break;

      case 'Y':
        show_cyls = true;
        break;

      case 'F':
        show_length_list = true;
        break;

      case 'u':
        cyls_only = true;
        show_cyls = true;
        break;

      case 't':
        show_moduli = true;
        show_cyls = true;
        break;

      case 'D':
        allow_long_cyls = true;
        break;

      case 'Z':
        heights_and_twists = true;
        show_cyls = true;
        break;

      case '?':
        ERR_RET("bad option");

      default:
        printf("?? getopt returned character code 0%o ??\n", c);
        ERR_RET("Bad Option");
    }
  }

  if (n_slices > MAX_SLICES) {
    std::cout << "Using " << MAX_SLICES << " slices\n";
    n_slices = MAX_SLICES;
  }

  if (nice_inc) {
    if (nice(nice_inc) == -1) {
      perror("failed to renice");
      abort();
    }
  }

  if (follow_depth < 0.0) {
    follow_depth = depth + 5; /* FIX ME */
  }

  if (closure) {
    retriangulate = false;
    perturb_euclidean = false;
    norescale = true;
    field_arithmetic = true;
    heights_and_twists = true;
  }

  if (file_arg == false) {
    if (optind != argc - 3 && optind != argc - 4) {
      ERR_RET("triangle: bad # of args");

    } else if (optind == argc - 3) {
      int a1, a2, a3;
      a1 = atoi(argv[optind++]);
      a2 = atoi(argv[optind++]);
      a3 = atoi(argv[optind++]);

      if (!no_field_arithmetic) {
        field_arithmetic = true;
      }
      S->BuildTriangle(a1, a2, a3);

    } else { /* optind == argc - 4 */
      int a1, a2, a3, a4;
      a1 = atoi(argv[optind++]);
      a2 = atoi(argv[optind++]);
      a3 = atoi(argv[optind++]);
      a4 = atoi(argv[optind++]);

      if (!no_field_arithmetic) {
        field_arithmetic = true;
      }
      S->BuildQuad(a1, a2, a3, a4);
    }
  } else {
    S->ReadComplex(filename_);
  }
  poly<bigrat> minimal_polynomial = NumberField<bigrat>::F->min_poly();
  std::cout << "Number field: min poly " << minimal_polynomial << endl;

  S->PrintAll(std::cout);
  S->CheckAllFaces();
  //    S->CheckAllVertices();

  S->make_pcomplexes();
  my_ostream debug_input_stream("input");
  S->NewDraw(debug_input_stream);
  debug_input_stream.close();

  char buf[1000];

  for (auto f = S->faces.begin(); f != S->faces.end(); ++f) {
    sprintf(buf, "face%d", (*f)->id());
    my_ostream* face_stream = new my_ostream(buf);
    S->make_pface(*f);
    S->NewDraw(*face_stream);
    face_stream->close();
    delete face_stream;
  }

  S->TriangulateAll();

  S->make_pcomplexes();
  my_ostream debug_stream("debug");
  S->NewDraw(debug_stream);
  debug_stream.close();

  S->Expunge();
  S->CheckAllFaces();

  S->CompleteSurface();
  S->BuildNeighborLists();
  S->set_area();

  S->CheckAllFaces();
  S->CheckAllVertices();
  S->StoreVertexOffsets();

  S->make_pcomplexes();
  my_ostream surface_stream("surface");
  S->NewDraw(surface_stream);
  surface_stream.close();

  if (retriangulate) {
    S->RetriangulateSurface();
    S->Expunge();
    S->CheckAllFaces();
    S->CheckAllVertices();
    S->StoreVertexOffsets();

    S->doExtraFlips();
    S->Expunge();
    S->CheckAllFaces();
    S->CheckAllVertices();
    S->StoreVertexOffsets();
  }

  if (retriangulate) {
    for (auto i = S->uedges.begin(); i != S->uedges.end(); ++i) {
      (*i)->internal = false;
      (*i)->from_triang = false;
    }
  }

  S->make_pcomplexes();
  my_ostream retriang_stream("retriang");
  S->NewDraw(retriang_stream);
  retriang_stream.close();

  std::cout << "Longest Edge: " << S->MaxEdge()->len() << endl;
  Dir<Point> shortest_dir;
  std::cout << "Looking for shortest saddle...";
  COORD shortest = S->MinSaddle(shortest_dir);
  std::cout << shortest << endl;
  COORD mv = min(0.1, shortest / 2);

  Point tmp_offset;

  if (perturb_euclidean) {
    for (auto i = S->vertices.begin(); i != S->vertices.end(); ++i) {
      if ((*i)->euclidean) {
        tmp_offset = Point(
            mv * std::uniform_real_distribution<double>(0, 1)(random_engine),
            mv * std::uniform_real_distribution<double>(0, 1)(random_engine));
        std::cout << "Moving: ";
        (*i)->Print(std::cout);
        std::cout << " offset " << tmp_offset << "\n";
        (*i)->MoveVertex(tmp_offset);
      }
    }
  }

  if (perturb_vertex != UNDEFINED) {
    if (offset == Point(UNDEFINED, UNDEFINED)) {
      offset = Point(
          mv * std::uniform_real_distribution<double>(0, 1)(random_engine),
          mv * std::uniform_real_distribution<double>(0, 1)(random_engine));
    }
    for (auto i = S->vertices.begin(); i != S->vertices.end(); ++i) {
      if ((*i)->id() == perturb_vertex) {
        std::cout << "Moving: ";
        (*i)->Print(std::cout);
        std::cout << " offset " << offset << "\n";
        (*i)->MoveVertex(offset);
      }
    }
  }

  if (perturb_conjugate) {
    fprintf(out_f, "Perturbing conjugates.....");
    if (perturb_magnitude < EPSILON) {
      perturb_magnitude = mv;
    }

    S->PerturbConjugates(perturb_magnitude);
    fprintf(out_f, "done\n");
  }

  if (perturb_all) {
    fprintf(out_f, "Perturbing surface...");
    if (perturb_magnitude < EPSILON) {
      perturb_magnitude = 0.5;
    }
    S->PerturbAll(perturb_magnitude);
    fprintf(out_f, "done\n");
  }

  S->make_pcomplexes();
  my_ostream perturbed_stream("perturbed");
  S->NewDraw(perturbed_stream);
  perturbed_stream.close();

  S->CheckAllFaces();
  S->CheckAllVertices();
  if (field_arithmetic) S->check_algebraicQ();

  if (field_arithmetic) {
    S->ClearDenominators();  // also rescales
    S->CheckAllFaces();
    S->CheckAllVertices();
    S->check_algebraicQ();

    S->check_algebraicI();
  }
  S->PrintAll(std::cout);

  S->StoreVertexOffsets();

  S->set_area();

  S->StatPrint(std::cout);

  ofstream fout("the_surface.dat");
  S->StatPrint(fout);
  S->PrintAll(fout);
  fout.close();

  if (!norescale) {
    S->set_scale_factor(S->get_scale_factor() / sqrt(S->get_area()));
  }

  depth /= S->get_scale_factor();
  follow_depth /= S->get_scale_factor();

  // not so sure about these
  // EPSILON /= S->get_scale_factor();
  // DELTA /= S->get_scale_factor();

  auto i = S->vertices.begin();

  for (; i != S->vertices.end(); ++i) {
    if (!((*i)->euclidean)) break;
  }
  if (i == S->vertices.end()) {
    ERR_RET("sweep: genus 1 not supported");
  }

  cout << "sweeping start V" << (*i)->id() << " depth = " << depth << endl;

  COORD GoalTotalAngle = (*i)->total_angle();
  auto first_edge = *((*i)->out_edges.begin());

  if (libflatsurf) {
    using FlatTriangulation =
        FlatTriangulation<VectorExactReal<NumberFieldTraits>>;
    auto flat_triangulation = static_cast<FlatTriangulation>(*S);
    std::cout << flat_triangulation << std::endl;

    const HalfEdge firstEdge = static_cast<HalfEdge>(*first_edge);
    HalfEdge sectorBegin = firstEdge;

    SaddleConf sc;

    do {
      using SaddleConnections =
          SaddleConnections<VectorExactReal<NumberFieldTraits>>;
      for (auto saddle_connection :
           SaddleConnections(flat_triangulation,
                             Bound(static_cast<long long>(ceil(depth * depth))),
                             sectorBegin)) {
        if (!Vertex::from(saddle_connection->source).relevant()) {
          // It would be good to have a proper notion of marked vertices
          // instead, but currently we rely on polygon's original concept of
          // "relevant"
          std::cout << "not relevant" << std::endl;
          continue;
        }

        sc.clear();

        auto direction = static_cast<
            flatsurf::VectorExactReal<exactreal::NumberFieldTraits>>(
            saddle_connection->vector);

        for (const HalfEdge e : flat_triangulation.edges()) {
          if (flat_triangulation.fromEdge(e).ccw(direction) ==
              flatsurf::CCW::CLOCKWISE) {
            continue;
          }
          if (flat_triangulation.fromEdge(flat_triangulation.nextAtVertex(e))
                  .ccw(direction) == flatsurf::CCW::COUNTERCLOCKWISE) {
            continue;
          }
          if (flat_triangulation.fromEdge(flat_triangulation.nextAtVertex(e))
                  .orientation(direction) != flatsurf::ORIENTATION::SAME) {
            continue;
          }

          const flatsurf::Vertex vertex =
              flatsurf::Vertex::source(e, flat_triangulation);
          const Vertex& _vertex(Vertex::from(vertex));
          if (!_vertex.relevant()) continue;
          if (_vertex.deleted()) continue;

          auto saddle_connections_in_same_direction = SaddleConnections(
              flat_triangulation,
              Bound(static_cast<long long>(ceil(follow_depth * follow_depth))),
              e);
          for (auto it = saddle_connections_in_same_direction.begin();
               it != saddle_connections_in_same_direction.end(); ++it) {
            auto saddle_connection_in_same_direction = *it;
            assert(saddle_connection_in_same_direction->source == vertex);
            auto ccw = saddle_connection_in_same_direction->vector.ccw(
                saddle_connection->vector);
            if (ccw == flatsurf::CCW::COLLINEAR) {
              Dir<Point> tmp_start, tmp_end;
              alg_tI tmp_algt;

              std::cout << "add_saddle: " << vertex << " "
                        << static_cast<flatsurf::VectorArb>(
                               static_cast<flatsurf::VectorExactReal<
                                   exactreal::NumberFieldTraits>>(
                                   saddle_connection_in_same_direction->vector))
                        << std::endl;

              // TODO: Implement me
              // sc.add_saddle(tmp_start, tmp_end, tmp_algt);

              break;
            } else {
              it.skipSector(-ccw);
            }
          }
        }
        if (show_lengths || show_cyls) {
          sc.renorm_lengths();
        }
      }
      sectorBegin = flat_triangulation.nextAtVertex(sectorBegin);
    } while (sectorBegin != firstEdge);
  } else {
    Dir<BigPointI> start_dir = Dir<BigPointI>(first_edge);
    S->Sweep<BigPointI>(depth, start_dir, GoalTotalAngle);

    S->issueFinalReport(smry, std::cout, (*i)->id());

    ofstream results_stream("final_results");
    S->issueFinalReport(smry, results_stream, (*i)->id());  // check
    results_stream.close();
  }

  delete S;

  exit(0);
}
