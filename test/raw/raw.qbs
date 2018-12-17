import qbs

Project {
	id: project
	name: "test_raw"

	CppApplication {
		name: "raw"
		consoleApplication: true
		cpp.defines: [
		].concat(project.parent.defines)
		cpp.includePaths: [
			"../include",
		].concat(project.parent.includePaths)
		cpp.staticLibraries: [
		].concat(project.parent.staticLibraries)
		cpp.dynamicLibraries: [
		].concat(project.parent.dynamicLibraries)
		files: [
			"raw.cpp",
			"../include/test.h",
		]

		Depends {
			name: "TinySerialization"
		}
	}

	CppApplication {
		name: "raw_stl"
		consoleApplication: true
		cpp.defines: [
		].concat(project.parent.defines)
		cpp.includePaths: [
			"../include",
		].concat(project.parent.includePaths)
		cpp.staticLibraries: [
		].concat(project.parent.staticLibraries)
		cpp.dynamicLibraries: [
		].concat(project.parent.dynamicLibraries)
		files: [
			"raw_stl.cpp",
			"../include/test.h",
		]

		Depends {
			name: "TinySerialization"
		}
	}
}
