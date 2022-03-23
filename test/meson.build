system = host_machine.system()

deps = [chronicle]

if system == 'windows'
  cpp = meson.get_compiler('cpp')
  synchapi = cpp.find_library('synchronization', required:true)
  deps += [synchapi]
endif

chronicle_test = executable('chronicle-test', 'test.cpp',
	dependencies: deps)
test('all', chronicle_test)

