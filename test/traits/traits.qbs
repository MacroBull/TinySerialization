import qbs

Project {
	id: project
	name: "test_traits"

	CppApplication {
		name: "traits"
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
			"traits.cpp",
			"../include/test.h",
		]

		Depends {
			name: "TinySerialization"
		}
	}

	CppApplication {
		name: "traits_stl"
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
			"traits_stl.cpp",
			"../include/test.h",
		]

		Depends {
			name: "TinySerialization"
		}
	}
}
