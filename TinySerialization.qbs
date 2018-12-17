import qbs

Project {
	id: rootProject
	references: [
		"test/test.qbs"
	]

	Product {
		id: rootProduct
		name: "TinySerialization"
//		version: [
//			vcs.type, vcs.repoState
//		].join("-")

		Group {
			id: headersGroup
			name: "Headers"
			fileTags: "hpp"
			prefix: "./include/serialization/"
			files: [
				"*.h",
			]
		}

		Export {
			cpp.cxxLanguageVersion: "c++11"
			cpp.defines: [
				"NAMESPACE=" + namespace,
			]
			cpp.includePaths: [
				"./include",
			]

			property string namespace: "TinySerialization"

			Group {
				id: exportHeadersGroup
				name: [
					rootProject.name, headersGroup.name
				].join(" ")
				fileTags: headersGroup.fileTags
				prefix: headersGroup.prefix
				files: headersGroup.files
			}

			Depends {
				name: "cpp"
			}

//			Depends {
//				name: "vcs"
//			}
		}
	}
}
