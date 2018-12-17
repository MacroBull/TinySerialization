import qbs

Project {
	id: testProject
	name: "test"
	references: [
		"traits/traits.qbs",
		"raw/raw.qbs",
	]

	property stringList defines: [
		"GTEST_LANG_CXX11",
	]
	property stringList includePaths: [
	]
	property stringList staticLibraries: [
		"/usr/lib/x86_64-linux-gnu/libgtest.a"
	]
	property stringList dynamicLibraries: [
		"glog",
		"pthread",
	]
}
