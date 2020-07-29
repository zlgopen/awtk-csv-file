import os
import app_helper as app

helper = app.Helper(ARGUMENTS);
helper.set_dll_def('src/csv_file.def').set_libs(['csv']).call(DefaultEnvironment)

CustomWidgetSConscriptFiles = []
SConscriptFiles = CustomWidgetSConscriptFiles + ['src/SConscript', 'demos/SConscript', 'tests/SConscript']
SConscript(SConscriptFiles)
