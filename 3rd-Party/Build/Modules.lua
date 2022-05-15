-- Visual Studio: Solution Items (multiple folders)
-- Source: https://github.com/premake/premake-core/issues/1061
require('vstudio')
premake.api.register {
  name = "workspace_files",
  scope = "workspace",
  kind = "list:keyed:list:string",
}
premake.override(premake.vstudio.sln2005, "projects", function(base, wks)
	if #wks.workspace_files > 0 then
		for _, folder in ipairs(wks.workspace_files) do
			for name, files in pairs(folder) do
				premake.push('Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "'..name..'", "'..name..'", "{' .. os.uuid("Solution Items:"..wks.name) .. '}"')
				premake.push("ProjectSection(SolutionItems) = preProject")

				for _, file in ipairs(files) do
				  file = path.rebase(file, ".", wks.location)
				  premake.w(file.." = "..file)
				end

				premake.pop("EndProjectSection")
				premake.pop("EndProject")
			end
		end
	end
	base(wks)
end)

-- Visual Studio: C/C++ 20 Conformance Mode Support
require('vstudio')
premake.api.register {
  name = "confromancemode",
  scope = "project",
  kind = "boolean",
}
premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
	local m = premake.vstudio.vc2010
	local calls = base(prj)

	if premake.project.iscpp(prj) then
		if prj.confromancemode then
			table.insertafter(calls, premake.xmlDeclaration,  function()
				premake.w('<ConformanceMode>true</ConformanceMode>')
			end)
		else
			table.insertafter(calls, premake.xmlDeclaration,  function()
				premake.w('<ConformanceMode>false</ConformanceMode>')
			end)
		end
	end
	return calls
end)

-- Visual Studio: C 17 Dialect Support
require('vstudio')
premake.api.register {
  name = "cdialectx",
  scope = "project",
  kind = "string",
}
premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
	local m = premake.vstudio.vc2010
	local calls = base(prj)

	if premake.project.iscpp(prj) then
		if prj.cdialectx then
			table.insertafter(calls, premake.xmlDeclaration,  function()
				premake.w('<LanguageStandard_C>stdc17</LanguageStandard_C>')
			end)
		end
	end
	return calls
end)

-- Visual Studio: C++ 20 Dialect Support
require('vstudio')
premake.api.register {
  name = "cppdialectx",
  scope = "project",
  kind = "string",
}
premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
	local m = premake.vstudio.vc2010
	local calls = base(prj)

	if premake.project.iscpp(prj) then
		if prj.cdialectx then
			table.insertafter(calls, premake.xmlDeclaration,  function()
				premake.w('<LanguageStandard>stdcpp20</LanguageStandard>')
			end)
		end
	end
	return calls
end)

-- Visual Studio: C++ 20 Modules Support
table.insert(premake.vstudio.vc2010.categories.ClCompile.extensions, ".cppm")
table.insert(premake.vstudio.vc2010.categories.ClCompile.extensions, ".cxx")
table.insert(premake.vstudio.vc2010.categories.ClCompile.extensions, ".ixx")

require('vstudio')
premake.api.register {
  name = "cppmodules",
  scope = "project",
  kind = "boolean",
}
premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
	local m = premake.vstudio.vc2010
	local calls = base(prj)

	if premake.project.iscpp(prj) then
		if prj.cppmodules then
			table.insertafter(calls, premake.xmlDeclaration,  function()
				premake.w('<EnableModules>true</EnableModules>')
			end)
		else
			table.insertafter(calls, premake.xmlDeclaration,  function()
				premake.w('<EnableModules>false</EnableModules>')
			end)
		end
		base(prj)
	end
	return calls
end)
