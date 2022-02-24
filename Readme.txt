2022/02/24-Added ballons tooltips
-------------------------------------------
2022/02/18-Updated help file
2022/02/18-Added a toolbar icon for reversing the filter
2022/02/18-Added more RNG limits
-------------------------------------------
2022/02/16-Corrected a bug when exporting the 'Filters' table.
2022/02/16-Corrected a bug when converting in grey.
2022/02/16-Added a groupbox for displaying image informations.
2022/02/16-Updated the help file.
2022/02/16-Replaced VirtualAlloc with malloc.
2022/02/16-Removed the option 'large Address Aware' in all the projects.
2022/02/16-Added a QR code in the help file and in the blog.
-------------------------------------------
2022/02/14-When the models listbox is empty it is not possible to add a new model.
2022/02/14-So when the the listbox is empty a new filter is created.
2022/02/14-Added more tests and more log infos when deleting a filter.
2022/02/14-Corrected a bug while buiding Export Directory Name.
2022/02/14-Correcting a bug when deleting a filter.
-------------------------------------------
2022/02/13-Corrected a bug when requesting the filters.
2022/02/13-Improved query for selecting rated/unrated filters.
2022/02/13-Added a global DC for all children windows except for thr Editor.
2022/02/13-Added a global buffer of 20Kb for updating and writing filters.
2022/02/13-Replaced some calls to lstrlen by replacing sqlite_snprintf by wsprintf.
2022/02/13-Replaced some calls to lstrlen by a loop and an Add.
2022/02/13-Added a warnig in "Data Filters." for not opening the file with Pelle's Editor.
-------------------------------------------
2022/02/11-Added verification function for Cfe2.ini file
2022/02/11-If Cfe2.ini does not exist, the function creates it.
-------------------------------------------
2022/02/10-Added a log file.
2022/02/10-Corrected bugs relatives to the export directory
2022/02/10-Paths are now in relative form to be "portable"
2022/02/10-The backup database always was selected even if one choose "No"
2022/02/10-but Cfe.ini was well written.
2022/02/10-All the actions relatives to the database are recorded into the log file.
-------------------------------------------
2022/02/09-When requesting a filter, the listbox is not empty.
2022/02/09-The export directory path is written in Cfe2.ini under the form of a relative path
2022/02/09-Like this Cfe really is "Portable". Remark of Gianluca Negrelli: "SyMenu".
2022/02/09-When exiting Cfe if "Reload Last File at Startup" is equal to "No"
2022/02/09-The corresponding line in Cfe2.ini is cleared.
2022/02/09-When exiting Cfe if "Backup Database" is set to "No"
2022/02/09-AND "Export the database when exit" is set to "No"
2022/02/09-The line, in Cfe2.ini, containing the export directory is cleared
2022/02/09-If a filename is specified under the key "Last File"
2022/02/09-AND if the specified file does not exist, the key "Last File" is cleared.
-------------------------------------------
2022/02/03-Corrected a bug in the export function.
2022/02/03-Corrected a bug in the import function.
2022/02/03-Modified icon for the slideshow.
2022/02/03-Added icon "Open File" in "Original" tab.
2022/02/03-Added the possibility to export the database in "Options" tab.
-------------------------------------------
2022/02/03-Rewrite began
-------------------------------------------
2022/02/02-Modified the tabs order. Original/Workspace/Editor/Result/Diaporama/Options.
2022/02/02-Integrated the result into Cfe, not a separate window.
2022/02/02-Integrated the diaporama into Cfe, not a separate window.
2022/02/02-The icon "Diaporama" is something like "On/Off" Flag.
2022/02/02-This is the reason it only appears while switching to the "Diaporama" tab.
2022/02/02-No keyboard action for the diaporama.
2022/02/02-When one click on a filter an icon is displayed for showing the filter state.
2022/02/02-Added two buttons Lock/Unlock on the toolbar.
2022/02/02-Added an icon for the "Inverse Filter".
2022/02/02-The button "Inverse Filter" is removed.
2022/02/02-Relooked the "Workspace" window.
2022/02/02-When the program is running if the client area is greather than 900,
2022/02/02-the result is shown on the right side.
2022/02/02-Added an icon for importing datas.
2022/02/02-Changed the "Apply" icon.
2022/02/02-Added comments into Cfe.ini
2022/02/02-Added a modeless dialog box when the program exits
2022/02/02-to show that it is working, doing export and/or vacuum.
2022/02/02-Updated the help file.
-------------------------------------------
2022/01/31-When exporting Filters.sql, the Divisor and the Bias where not a the good place.
-------------------------------------------
2022/01/26-Added rescue filters. When the database is created, it is not empty.
2022/01/26-Added a function to initialize the tables "Filters" and "Convolution".
2022/01/26-Version 1.0.0.9546
2022/01/26-Added 528 filters.
-------------------------------------------
2022/01/25-Regardless of the selected tab, the toolbar buttons are hidden according to the role of the tab.
2022/01/25-The export directory is stored into Cfe.ini
2022/01/25-There is an option in Tab#4 for specifying it.
2022/01/25-Added messages into the languages files.
2022/01/25-Added asm functions.
-------------------------------------------
2022/01/24-The button "Inverse Filter" did not change the ratings combobox to 0.
2022/01/24-When clicking on "New Filter", the previous comment is not deleted.
2022/01/24-When the image is loaded at startup, the image size was not written in the statusbas
2022/01/24-Added examples of SOBEL, PREWITT and LAPLACE filters into the help file.
2022/01/24-The files exported by Cfe are created in a folder named "Cfe" into "My Documents".
2022/01/24-The database is copied into a folder named "Cfe" into "My Documents".
-------------------------------------------
2022/01/23-Added EULA for Cfe
2022/01/23-Modified Cfe.chm
2022/01/23-Modified version number to 1.0.0.9377
2022/01/23-Corrected a bug when updating a 29x29 filter. Final buffer too small.
2022/01/23-When an option was called, the raing combobox kept ranking if therewas one for the previous filter.
2022/01/23-Sometimes the modeless dialogbox "Modeless" stays on screen and did not leave.
-------------------------------------------
2022/01/22-The file "Cfe.ini" is overwritten by the installer.
2022/01/22-Added an option "Backup Database".
2022/01/22-Relooking of the ToolBox.
-------------------------------------------
2022/01/21-Fields "Average" and "StdDeviation" renamed as "UnUsed_1 & 2".
2022/01/21-Corrected a bug in "OpenThisFile".
2022/01/21-Corrected a bug in the Randomizer.
2022/01/21-Corrected a bug in the "Filter" creation table.
-------------------------------------------
2022/01/20-Added an option "Circular Rotate Top" in the editor.
2022/01/20-Added an option "Circular Rotate Bottom" in the editor.
2022/01/20-Added an option "Circular Rotate Left" in the editor.
2022/01/20-Added an option "Circular Rotate Right" in the editor.
2022/01/20-Added an option "Shift to Top" in the editor.
2022/01/20-Added an option "Shift to Bottom" in the editor.
2022/01/20-Added an option "Shift to Left" in the editor.
2022/01/20-Added an option "Shift to Right" in the editor.
2022/01/20-Added an option "Rotate to Left" in the editor.
2022/01/20-Added an option "Rotate to Right" in the editor.
2022/01/20-The exported files are stored into "My Documents". The files are not exported when exit.
2022/01/20-Created themes for Cfe.
2022/01/20-Removed the "Chronometer" icon, replaced by "Export" icon.
2022/01/20-Modified the text in the About dialogbox.
2022/01/20-Changed Zonar with Zvonar and added Pierre Emmanuel Gougelet.
-------------------------------------------
2022/01/19-Resolved warnings when compiling. #2804
2022/01/19-Added an option "Horizontal Miror" in the editor.
2022/01/19-Added an option "Vertical Miror" in the editor.
2022/01/19-Modifier error message #15
2022/01/19-Added possibility to maximize the toolbox;
2022/01/19-Replaced three icons (Delete, Quit and About)
-------------------------------------------
2022/01/17-Added "BEGIN/END TRANSACTION" in the SQLite exported files.
2022/01/17-Added an option "Insert column after" in the editor.
2022/01/17-Changed the order of the two insert column icons in the toolbar.
2022/01/17-Changed the order of the delete row/colum icons in the toolbar.
2022/01/17-Added an option "Insert row below" in the editor.
2022/01/17-Added an option "Insert row above" in the editor.
2022/01/17-Main timer reduced to 2 seconds rather than 5.
2022/01/17-Modified text when ellapsed time < 1 second.
2022/01/17-Added an option "Delete Column" in the editor.
2022/01/17-Added an option "Delete Row" in the editor.
2022/01/17-Added the filter name when displaying the diaporama.
-------------------------------------------
2022/01/16-Corrected a bug in the export filters function.
2022/01/16-Added an option "Insert column before" in the editor.
-------------------------------------------
2022/01/15-Added option in the tab #4.
2022/01/15-Added toolbar for the editor.
2022/01/15-Modified database structure.
2022/01/15-Corrected some bugs.
2022/01/15-Changed the "Randomize" icon in the toolbar.
-------------------------------------------
2022/01/11-Solved Virtual Memory Crash when using "SaveAllFilters".
-------------------------------------------
2022/01/10-Cfe records the last edited file name
2022/01/10-Cfe allows to load the last file at startup
2022/01/10-Added missing tooltips
2022/01/10-Modified translations files
2022/01/10-Only the file name is written on the main window
2022/01/10-If the database is too big the VACUUM command could take a long time
2022/01/10-It is possible to skip it via Cfe.ini
2022/01/10-When clicking on the toolbox it activates the display window and vice-versa
2022/01/10-Added the version number on the statusbar
-------------------------------------------
2022/01/09-Added queries for selecting filters:
2022/01/09-RADIO_BLACK_WHITE + RADIO_RATED_UNRATED = SIZE
2022/01/09-RADIO_BLACK_WHITE + RADIO_RATED
2022/01/09-RADIO_BLACK_WHITE + RADIO_UNRATED
2022/01/09-RADIO_BLACK + RADIO_RATED_UNRATED = RADIO_BLACK
2022/01/09-RADIO_BLACK + RADIO_RATED
2022/01/09-RADIO_BLACK + RADIO_UNRATED
2022/01/09-RADIO_WHITE + RADIO_RATED_UNRATED = RADIO_WHITE
2022/01/09-RADIO_WHITE + RADIO_RATED
2022/01/09-RADIO_WHITE + RADIO_UNRATED
2022/01/09-SELECT Filters.RecordNumber,A.Name FROM [Filters] INNER JOIN [Asteroids] AS A ON Filters.NameID = A.RecordNumber WHERE ([ColsNumber] = %d);
2022/01/09-SELECT Filters.RecordNumber,A.Name FROM [Filters] INNER JOIN [Asteroids] AS A ON Filters.NameID = A.RecordNumber WHERE (([ColsNumber] = %d) AND ([Rate] > 0));
2022/01/09-SELECT Filters.RecordNumber,A.Name FROM [Filters] INNER JOIN [Asteroids] AS A ON Filters.NameID = A.RecordNumber WHERE (([ColsNumber] = %d) AND ([Rate] < 1))
2022/01/09-SELECT Filters.RecordNumber,A.Name FROM [Filters] INNER JOIN [Asteroids] AS A ON Filters.NameID = A.RecordNumber WHERE ([ColsNumber] = %d) AND ([BlackAndWhite] < 1));
2022/01/09-SELECT Filters.RecordNumber,A.Name FROM [Filters] INNER JOIN [Asteroids] AS A ON Filters.NameID = A.RecordNumber WHERE (([ColsNumber] = %d) AND ([Rate] > 0) AND ([BlackAndWhite] < 1));
2022/01/09-SELECT Filters.RecordNumber,A.Name FROM [Filters] INNER JOIN [Asteroids] AS A ON Filters.NameID = A.RecordNumber WHERE (([ColsNumber] = %d) AND ([Rate] < 1) AND ([BlackAndWhite] < 1));
2022/01/09-SELECT Filters.RecordNumber,A.Name FROM [Filters] INNER JOIN [Asteroids] AS A ON Filters.NameID = A.RecordNumber WHERE (([ColsNumber] = %d) AND ([BlackAndWhite] > 0));
2022/01/09-SELECT Filters.RecordNumber,A.Name FROM [Filters] INNER JOIN [Asteroids] AS A ON Filters.NameID = A.RecordNumber WHERE (([ColsNumber] = %d) AND ([Rate] > 0) AND ([BlackAndWhite] > 0));
2022/01/09-SELECT Filters.RecordNumber,A.Name FROM [Filters] INNER JOIN [Asteroids] AS A ON Filters.NameID = A.RecordNumber WHERE (([ColsNumber] = %d) AND ([Rate] < 1) AND ([BlackAndWhite] > 0));
-------------------------------------------
2022/01/09-Corrected a bug in the ReadFilter function
2022/01/09-Corrected a bug in the UpdateFilter function
2022/01/09-UpdateFilter requires a password
-------------------------------------------
2022/01/07-Added the combobox Ratings.
2022/01/07-Reduced the number of parts of the statusbar.
2022/01/07-The ellapse time is displayed on the right part of the statusbar.
-------------------------------------------
2022/01/06-7UpCfe installs a version that does not allow the user to delete
2022/01/06-models and level 1 filters. The user, only can delete the filters
2022/01/06-he created.
2022/01/06-Modified the name ot the languages files and idem for the language files in the project
2022/01/06-I did this because it became dangerouds when deleting files. Specialy for the Spanish files.
2022/01/06-All the files for the languages beins with "Lng_".
2022/01/06-The DLL languages files are names "Lng_{xxxxxx}.cfe, example Lng_French.cfe.
2022/01/06-There was a small bug in the reset function, now correct.
2022/01/06-Added an icon on the toolbar for the reinitialize option.
2022/01/06-Added a trackbar to give the possiblity to add or to sub a value from the ones into the editor.
2022/01/06-Added an "About" icon on the toolBar.
2022/01/06-Added an option to measure how much time takes a filter to build the image.
2022/01/06-When choosing a filter size, a new one is create randomly rather than having a black image.
2022/01/06-By default the divisor was set to the total numbers of columns to avoid a logical bug.
2022/01/06-Now it is set to 1 to avaoid black images.
2022/01/06-The export options is reserved to the maintainers only.
2022/01/06-The file ToolBar.pfi is used by PhotoFiltre to build the toolbar.bmp file
2022/01/06-Removed comment generating a manifest file into Cfe.c
2022/01/06-Remove the two manifests present in the Cfe folder.
2022/01/06-Now there is only one manifest : Cfe.xml use as a resource.
2022/01/06-Removed comment generating a manifest file into Cfe.c
2022/01/06-Removed two long text displayed on the first toolbox window.
2022/01/06-Now they are in the help file.
2022/01/06-Added a combobox specifying the limits for the random number generator.
2022/01/06-Removed buttons * 2 and / 2.
2022/01/06-Removed unused tooltips.
2022/01/06-Modified the random generator algotithm.
2022/01/06-Now Cfe uses Frankie's one.
2022/01/06-Corrected a bug in the chronometer.
2022/01/06-Added PushFilter at the beginning of many function rather than the end.
2022/01/06-Modified table "Filters" structure.
2022/01/06-Added 3 fields. Only one will be used, the others are reserved for future utilization.
-------------------------------------------
