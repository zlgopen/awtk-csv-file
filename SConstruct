import os
import scripts.app_helper as app

helper = app.Helper(ARGUMENTS);
helper.set_libs(['csv']).call(DefaultEnvironment)

CustomWidgetSConscriptFiles = []
SConscriptFiles = CustomWidgetSConscriptFiles + ['src/SConscript', 'demos/SConscript', 'tests/SConscript']
helper.SConscript(SConscriptFiles)
