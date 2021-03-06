// COLMAP - Structure-from-Motion and Multi-View Stereo.
// Copyright (C) 2017  Johannes L. Schoenberger <jsch at inf.ethz.ch>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "mvs/patch_match.h"
#include "util/option_manager.h"

using namespace colmap;

int main(int argc, char* argv[]) {
  InitializeGlog(argv);

  std::string workspace_path;
  std::string workspace_format;
  std::string pmvs_option_name = "option-all";

  OptionManager options;
  options.AddDenseMapperOptions();
  options.AddRequiredOption("workspace_path", &workspace_path);
  options.AddRequiredOption("workspace_format", &workspace_format);
  options.AddDefaultOption("pmvs_option_name", pmvs_option_name,
                           &pmvs_option_name);

  if (!options.Parse(argc, argv)) {
    return EXIT_FAILURE;
  }

  if (options.ParseHelp(argc, argv)) {
    return EXIT_SUCCESS;
  }

  if (workspace_format != "COLMAP" && workspace_format != "PMVS") {
    std::cout << "ERROR: Invalid `workspace_format` - supported values are "
                 "'COLMAP' or 'PMVS'."
              << std::endl;
    return EXIT_FAILURE;
  }

  mvs::PatchMatchController controller(
      options.dense_mapper_options->patch_match, workspace_path,
      workspace_format, pmvs_option_name,
      options.dense_mapper_options->max_image_size);

  controller.Start();
  controller.Wait();

  return EXIT_SUCCESS;
}
