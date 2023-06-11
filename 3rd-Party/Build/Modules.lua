-- Visual Studio: Solution Items (multiple folders)
-- Source: https://github.com/premake/premake-core/issues/1061
require("vstudio")
premake.api.register {
	name = "solutionitems",
	scope = "workspace",
	kind = "table",
}

function DetectSolutionItems(wks, items)
	for name, files in pairs(items) do
		if type(name) ~= "number" then
			local children = false

			premake.push('Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "'..name..'", "'..name..'", "{' .. os.uuid("Solution Items:"..wks.name..":"..name) .. '}"')
			premake.push("ProjectSection(SolutionItems) = preProject")
            
			for _, file in pairs(files) do
				if type(_) == "number" and type(file) == "string"  then
					for _, file in ipairs(os.matchfiles(file)) do
						file = path.rebase(file, ".", wks.location)
						premake.w(file.." = "..file)
					end
                else
					children = true
                    print(string.format("%s: %s", name, files))
				end
			end

			premake.pop("EndProjectSection")
			premake.pop("EndProject")

			if children then
				DetectSolutionItems(wks, files)
			end
        else
		end
	end
end

function DetectNestedSolutionItems(wks, parent, items)
    local parent_uuid
    if parent == "Solution Items" then
        parent_uuid = os.uuid("Solution Items:"..wks.name)
    else
        parent_uuid = os.uuid("Solution Items:"..wks.name..":"..parent)
    end
    
	for name, file in pairs(items) do
		if type(name) ~= "number" then
			project_uuid = os.uuid("Solution Items:"..wks.name..":"..name)
			premake.w("{%s} = {%s}", project_uuid, parent_uuid)
			DetectNestedSolutionItems(wks, name, file)
		end
	end
end

premake.override(premake.vstudio.sln2005, "projects", function(base, wks)
	if wks.solutionitems and #wks.solutionitems > 0 then
		premake.push('Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "Solution", "Solution", "{' .. os.uuid("Solution:"..wks.name) .. '}"')
		premake.push("ProjectSection(SolutionItems) = preProject")

		for _, file in pairs(wks.solutionitems) do
        
			if type(_) == "number" and type(file) == "string" then
                -- Root
				for _, file in ipairs(os.matchfiles(file)) do
					file = path.rebase(file, ".", wks.location)
					premake.w(file.." = "..file)
				end
			end
		end
		premake.pop("EndProjectSection")
		premake.pop("EndProject")
		
		DetectSolutionItems(wks, wks.solutionitems)
	end
	base(wks)
end)

premake.override(premake.vstudio.sln2005, "nestedProjects", function(base, wks)
    premake.push("GlobalSection(NestedProjects) = preSolution")
	local tr = premake.workspace.grouptree(wks)
	if premake.tree.hasbranches(tr) then
		premake.tree.traverse(tr, {
			onnode = function(n)
				if n.parent.uuid then
					premake.w("{%s} = {%s}", (n.project or n).uuid, n.parent.uuid)
				end
			end
		})
	end
	
    parent = "Solution Items"
	DetectNestedSolutionItems(wks, parent, wks.solutionitems)
	premake.pop("EndGlobalSection")
end)

-- Visual Studio: C/C++ 20 Conformance Mode Support (was fixed since 5.0-beta.1)
----require('vstudio')
----premake.api.register {
----  name = "confromancemode",
----  scope = "project",
----  kind = "boolean",
----}
----premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
----	local m = premake.vstudio.vc2010
----	local calls = base(prj)
----
----	if premake.project.iscpp(prj) then
----		if prj.confromancemode then
----			table.insertafter(calls, premake.xmlDeclaration,  function()
----				premake.w('<ConformanceMode>true</ConformanceMode>')
----			end)
----		else
----			table.insertafter(calls, premake.xmlDeclaration,  function()
----				premake.w('<ConformanceMode>false</ConformanceMode>')
----			end)
----		end
----	end
----	return calls
----end)

-- Visual Studio: C 17 Dialect Support (was fixed since 5.0-beta.2)
----require('vstudio')
----premake.api.register {
----  name = "cdialectx",
----  scope = "project",
----  kind = "string",
----}
----premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
----	local m = premake.vstudio.vc2010
----	local calls = base(prj)
----
----	if premake.project.iscpp(prj) then
----		if prj.cdialectx then
----			table.insertafter(calls, premake.xmlDeclaration,  function()
----				premake.w('<LanguageStandard_C>stdc17</LanguageStandard_C>')
----			end)
----		end
----	end
----	return calls
----end)

-- Visual Studio: C++ 20 Dialect Support (was fixed since 5.0-beta.2)
----require('vstudio')
----premake.api.register {
----  name = "cppdialectx",
----  scope = "project",
----  kind = "string",
----}
----premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
----	local m = premake.vstudio.vc2010
----	local calls = base(prj)
----
----	if premake.project.iscpp(prj) then
----		if prj.cdialectx then
----			table.insertafter(calls, premake.xmlDeclaration,  function()
----				premake.w('<LanguageStandard>stdcpp20</LanguageStandard>')
----			end)
----		end
----	end
----	return calls
----end)

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
