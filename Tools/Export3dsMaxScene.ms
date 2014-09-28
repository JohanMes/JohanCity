utility exportscene "3dsMax Scene Converter" (
	
	edittext folderprefix "Folder:" text:"maxexport"
	
	button exportscn "Export scene" offset:[0,20]
	checkbox includetransforms "Use transforms" offset:[25,0]
	progressbar scnprogress color:green

	button exportobj "Export objects" offset:[0,20]
	progressbar objprogress color:green
	
	global workingdir = "C:\\Users\\Johan\\Desktop\\Data\\"

	function WriteSCN = (
		
		-- Paden bijhouden, copy van/naar WriteOBJ
		datafolder = ""
		if(folderprefix.text != "") then ( -- geen loze slash gebruiken
			datafolder = folderprefix.text + "\\"
		)

		scenedir = workingdir + "Scenes\\" + datafolder
		materialdir = workingdir + "Materials\\" + datafolder
		modeldir = workingdir + "Models\\" + datafolder

		iterator = 0
		
		makeDir scenedir
		makeDir materialdir
		
		scenefilename = scenedir + "MaxExport.script"
		scenefile = createFile scenefilename
		
		-- Maak een zon (dan zie je nog is wat)
		format "SetSunlight(1.4,1.4,1.2)\n\n" to:scenefile
		
		-- Stel de tijd fatsoenlijk in
		format "SetTime(12,00)\n" to:scenefile
		format "SetTimeMulti(1)\n\n" to:scenefile
		
		-- Zet de camera ergens neer
		format "SetCameraPos(0,1.85,0 0 0)\n\n" to:scenefile
		format "SetCameraAngleDeg(0,0)\n\n" to:scenefile
		
		-- Maak een standaardgrond aan
		format "SetGround(\"Ground.mtl\",\"Heightmaps\Wit.bmp\",8000,0,0,1000)\n\n" to:scenefile

		-- En maak een skybox
		format "AddObject(\"Scene\Skybox8000.obj\",\"Skybox2.mtl\",0,0,0,0,0,0,1)\n" to:scenefile
		format "AddObject(\"Scene\Sun.obj\",\"Sun.mtl\",0,0,0,0,0,0,1)\n\n" to:scenefile

		-- Maak als laatst een lijst met de exports
		for obj in geometry do (
			
			-- Vooruitgang
			iterator = iterator+1
			scnprogress.value = 100.0f*iterator/geometry.count
			
			-- tmp, haal nummer weg...
			internalname = obj.name
		--	if findString internalname "Barrel" != undefined then (
		--		internalname = "Barrel"
		--	)
			
			-- Kopietje uit WriteOBJ
			modelfilename = internalname + ".obj"
			modelfullpath = modeldir + modelfilename
		
			-- Schrijf naar scnfile
			if(obj.material != undefined and classof(obj.material) != MultiMaterial and obj.material.DiffuseMap != undefined) then (
				
				 -- split by \ so filename ends up at the end
				tokens1 = filterString obj.material.DiffuseMap.Filename "\\"
				
				-- split filename (part after last \) by . again to obtain file name without extensiom
				tokens2 = filterString tokens1[tokens1.count] "."
				
				materialfilename = tokens2[1] + ".mtl"
				materialfullpath = materialdir + materialfilename
					
				-- Schrijf matfile
				materialfile = createFile materialfullpath
				if(obj.material.SpecularMap == undefined and obj.material.Bumpmap == undefined) then (
					format "diffuse % 1\nshader PerPixel" obj.material.DiffuseMap.Filename to:materialfile
				) else if(obj.material.SpecularMap != undefined and obj.material.Bumpmap == undefined) then (
					format "diffuse % 1\nspecular % 1 1\nshader PerPixelSpecular" obj.material.DiffuseMap.Filename obj.material.SpecularMap.Filename to:materialfile
				) else if(obj.material.SpecularMap == undefined and obj.material.Bumpmap != undefined) then (
					format "diffuse % 1\nnormal %\nshader PerPixelNormal" obj.material.DiffuseMap.Filename obj.material.BumpMap.Filename to:materialfile
				) else (
					format "diffuse % 1\nspecular % 1 1\nnormal %\nshader PerPixelNormalSpecular" obj.material.DiffuseMap.Filename obj.material.SpecularMap.Filename obj.material.BumpMap.Filename to:materialfile
				)
				close materialfile
				
				-- Geef een pad relatief aan Models\ of Materials\ op
				modelfilename = datafolder + modelfilename
				materialfilename = datafolder + materialfilename
				
				-- En exporteren maar
				if(includetransforms.checked) then (
					euler = obj.rotation as eulerangles
					format "AddObject(\"%\",\"%\",%,%,%,%,%,%,%)\n" modelfilename materialfilename obj.pos.x obj.pos.z obj.pos.y euler.x euler.z euler.y obj.scale.x to:scenefile
				) else (
					format "AddObject(\"%\",\"%\",0,0,0,0,0,0,1)\n" modelfilename materialfilename to:scenefile
				)
			)
		)
		close scenefile
	)

	function WriteOBJ = (
		
		-- Paden bijhouden, copy van/naar WriteOBJ
		datafolder = folderprefix.text + "\\"
		scenedir = workingdir + "Scenes\\" + datafolder
		materialdir = workingdir + "Materials\\" + datafolder
		modeldir = workingdir + "Models\\" + datafolder
		
		iterator = 0
		
		makeDir modeldir
		
		exportedmodels = #()
		for obj in geometry do (
			
			-- Vooruitgang
			iterator = iterator+1
			objprogress.value = 100.0f*iterator/geometry.count
			
			-- Kopietje uit WriteOBJ
			modelfilename = obj.name + ".obj"
			modelfullpath = modeldir + modelfilename
			
			-- Hebben we deze al op de schijf opgeslagen? Zo nee, exporteren met correctie
			write = true
			for i = 1 to exportedmodels.count do (
				if(exportedmodels[i] == modelfullpath) then (
					write = false
					break
				)
			)
			if(write) then (
				
				-- handednesscorrectie
				addmodifier obj (Mirror())
				obj.modifiers[1].mirror_axis = 1
				obj.modifiers[1].Mirror_Center.Position = -obj.Position
				
				-- Exporteren met correctie
				select obj
				exportFile(modelfullpath) #noPrompt selectedOnly:true
				deletemodifier obj 1
				
				append exportedmodels modelfullpath
			)
		)
	)
	
	on exportall pressed do (
		WriteSCN()
		WriteOBJ()
	)
	
	on exportscn pressed do (
		WriteSCN()
	)

	on exportobj pressed do (
		WriteOBJ()
	)
)