<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="13008000">
	<Property Name="Instrument Driver" Type="Str">True</Property>
	<Property Name="NI.Project.Description" Type="Str">This project is used by developers to edit API and example files for LabVIEW Plug and Play instrument drivers.</Property>
	<Item Name="My Computer" Type="My Computer">
		<Property Name="CCSymbols" Type="Str">OS,Win;CPU,x86;</Property>
		<Property Name="NI.SortType" Type="Int">3</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="Examples" Type="Folder">
			<Item Name="Thorlabs MC2000B Read &amp; Write demo.vi" Type="VI" URL="/&lt;instrlib&gt;/Thorlabs_MC2000B/Examples/Thorlabs MC2000B Read &amp; Write demo.vi"/>
		</Item>
		<Item Name="Library" Type="Folder">
			<Item Name="MC2000CommandLibWin32.dll" Type="Document" URL="/&lt;instrlib&gt;/Thorlabs_MC2000B/Library/MC2000CommandLibWin32.dll"/>
			<Item Name="MC2000CommandLibWin64.dll" Type="Document" URL="/&lt;instrlib&gt;/Thorlabs_MC2000B/Library/MC2000CommandLibWin64.dll"/>
		</Item>
		<Item Name="Thorlabs_MC2000B.lvlib" Type="Library" URL="/&lt;instrlib&gt;/Thorlabs_MC2000B/Thorlabs_MC2000B.lvlib"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
			</Item>
			<Item Name="instr.lib" Type="Folder">
				<Item Name="MC2000CommandLibWin64.dll" Type="Document" URL="/&lt;instrlib&gt;/Thorlabs_MC2000B/Library/MC2000CommandLibWin64.dll"/>
			</Item>
			<Item Name="MC2000CommandLibWin64.dll" Type="Document" URL="../../../../../../Program Files/National Instruments/LabVIEW 2011/instr.lib/Thorlabs_MC2000B/Library/MC2000CommandLibWin64.dll"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="Thorlabs_MC2000B" Type="Zip File">
				<Property Name="Absolute[0]" Type="Bool">false</Property>
				<Property Name="BuildName" Type="Str">Thorlabs_MC2000B</Property>
				<Property Name="Comments" Type="Str"></Property>
				<Property Name="DestinationID[0]" Type="Str">{82211681-5683-47B9-BC77-25A95CA6AF3B}</Property>
				<Property Name="DestinationItemCount" Type="Int">1</Property>
				<Property Name="DestinationName[0]" Type="Str">Destination Directory</Property>
				<Property Name="IncludedItemCount" Type="Int">1</Property>
				<Property Name="IncludedItems[0]" Type="Ref">/My Computer</Property>
				<Property Name="IncludeProject" Type="Bool">true</Property>
				<Property Name="Path[0]" Type="Path">../../builds/Thorlabs_MC2000B_LabVIEWSDK.zip</Property>
				<Property Name="ZipBase" Type="Str">NI_zipbasedefault</Property>
			</Item>
		</Item>
	</Item>
</Project>
