 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <boca.h>

#include "chooser_files.h"
#include "chooser_files_tree.h"
#include "chooser_files_utilities.h"

BoCA::ChooserFiles::ChooserFiles() : Chooser("Files")
{
	list_directories= new ListBox(Point(7,7), Size(150, 150));

	div_split	= new Divider(160, OR_VERT);

	edit_directory	= new EditBox(NIL, Point(165, 7), Size(100, 0));
	edit_directory->Deactivate();

	list_files	= new ListBox(Point(165, 34), Size(100, 150));
	list_files->AddTab(I18n::Get()->TranslateString("File"));
	list_files->Deactivate();
	list_files->onSelectEntry.Connect(&ChooserFiles::OnSelectFile, this);

	text_nofiles	= new Text("no audio files found", Point());
	text_nofiles->SetFont(Font(Font::Default, 12, Font::Bold, 0, Setup::GrayTextColor));

	btn_save	= new Button("Save", NIL, Point(176, 30), Size());
	btn_save->SetOrientation(OR_LOWERRIGHT);
	btn_save->Deactivate();
	btn_save->onAction.Connect(&ChooserFiles::OnSave, this);

	btn_saveall	= new Button("Save all", NIL, Point(88, 30), Size());
	btn_saveall->SetOrientation(OR_LOWERRIGHT);
	btn_saveall->Deactivate();
	btn_saveall->onAction.Connect(&ChooserFiles::OnSaveAll, this);

	ChooserFilesTree::onSelectDirectory.Connect(&ChooserFiles::OnSelectDirectory, this);

#ifdef __WIN32__
	/* Add local drives.
	 */
	for (Int i = 0; i < 26; i++)
	{
		char		 name[4] = { 'A' + i, ':', '\\', 0};
		Directory	 drive(name);

		if (drive.Exists())
		{
			Tree		*tree = new ChooserFilesTree(drive);

			trees.Add(tree);

			list_directories->Add(tree);
		}
	}
#else
	/* Add root directory.
	 */
	Tree	*root = new ChooserFilesTree(Directory("/"));

	trees.Add(root);

	list_directories->Add(root);
#endif

	Add(list_directories);

	Add(div_split);

	Add(edit_directory);
	Add(list_files);
	Add(text_nofiles);

	Add(btn_save);
	Add(btn_saveall);

	onChangeSize.Connect(&ChooserFiles::OnChangeSize, this);
}

BoCA::ChooserFiles::~ChooserFiles()
{
	list_files->RemoveAllEntries();

	foreach (Tree *tree, trees) DeleteObject(tree);

	DeleteObject(list_directories);

	DeleteObject(div_split);

	DeleteObject(edit_directory);
	DeleteObject(list_files);
	DeleteObject(text_nofiles);

	DeleteObject(btn_save);
	DeleteObject(btn_saveall);
}

/* Called when component canvas size changes.
 * ----
 */
Void BoCA::ChooserFiles::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	list_directories->SetHeight(clientSize.cy - 15);

	edit_directory->SetWidth(clientSize.cx - 158 - 15);
	list_files->SetSize(Size(clientSize.cx - 158 - 15, clientSize.cy - 72));
	text_nofiles->SetPosition(list_files->GetPosition() + Point((list_files->GetWidth() - text_nofiles->textSize.cx) / 2, (list_files->GetHeight() - text_nofiles->textSize.cy) / 2));
}

/* Called when a directory entry is selected.
 * ----
 * Reads files in the selected directory and adds them to the files list.
 */
Void BoCA::ChooserFiles::OnSelectDirectory(const Directory &directory)
{
	if (String(directory) == edit_directory->GetText()) return;

	GetSupportedFileExtensions();

	if (modified.Length() > 0)
	{
		if (IDYES == Dialogs::QuickMessage("There are unsaved files in this directory. Would you like to save them now?", "Save changes", MB_YESNO, IDI_QUESTION)) OnSaveAll();
	}

	edit_directory->SetText(String(directory).Append(Directory::GetDirectoryDelimiter()));

	if (IsRegistered()) container->GetDrawSurface()->StartPaint(Rect(list_files->GetRealPosition(), list_files->GetSize()));

	list_files->RemoveAllEntries();

	const Array<File>	&files = directory.GetFiles();

	foreach (File file, files)
	{
		String	 filename = file.GetFileName();
		String	 extension = filename.Tail(filename.Length() - filename.FindLast(".") - 1).ToLower();

		if (extensions.Get(extension.ComputeCRC32()) != NIL) list_files->AddEntry(filename);
	}

	if (list_files->Length() == 0)
	{
		list_files->Deactivate();

		if (text_nofiles->IsVisible())	text_nofiles->Paint(SP_PAINT);
		else				text_nofiles->Show();
	}
	else
	{
		text_nofiles->Hide();
		list_files->Activate();
	}

	if (IsRegistered()) container->GetDrawSurface()->EndPaint();

	btn_save->Deactivate();
	btn_saveall->Deactivate();

	onSelectNone.Emit();

	tracks.RemoveAll();
	modified.RemoveAll();
}

/* Called when a file entry is selected.
 * ----
 * Tries to open the file and generate a track object.
 */
Void BoCA::ChooserFiles::OnSelectFile(ListEntry *entry)
{
	if (tracks.Get(list_files->GetSelectedEntryNumber()) != NIL)
	{
		if (modified.Get(list_files->GetSelectedEntryNumber()) != NIL)	btn_save->Activate();
		else								btn_save->Deactivate();

		onSelectTrack.Emit(tracks.Get(list_files->GetSelectedEntryNumber()));

		return;
	}

	File			 file(entry->GetText(), edit_directory->GetText());
	DecoderComponent	*decoder = ChooserFilesUtilities::CreateDecoderComponent(file);

	if (decoder == NIL)
	{
		Utilities::ErrorMessage(String(BoCA::I18n::Get()->TranslateString("Unable to open file: %1\n\nError: %2")).Replace("%1", file.GetFileName()).Replace("%2", BoCA::I18n::Get()->TranslateString("Unknown file type")));

		return;
	}

	Track	 track;
	Error	 error = decoder->GetStreamInfo(file, track);
	String	 errorString = decoder->GetErrorString();

	Registry::Get().DeleteComponent(decoder);

	if (error == Error())
	{
		Utilities::ErrorMessage(String(BoCA::I18n::Get()->TranslateString("Unable to open file: %1\n\nError: %2")).Replace("%1", file.GetFileName()).Replace("%2", BoCA::I18n::Get()->TranslateString(errorString)));

		return;
	}

	track.origFilename = file;

	btn_save->Deactivate();

	onSelectTrack.Emit(track);

	tracks.Add(track, list_files->GetSelectedEntryNumber());
}

/* Called when a track is modified.
 * ----
 * Finds the track and marks it as modified.
 */
Void BoCA::ChooserFiles::OnModifyTrack(const Track &track)
{
	for (Int i = 0; i < tracks.Length(); i++)
	{
		if (tracks.GetNth(i).GetTrackID() == track.GetTrackID())
		{
			Font	 font = list_files->GetNthEntry(tracks.GetNthIndex(i))->GetFont();

			font.SetColor(Color(0, 0, 255));

			list_files->GetNthEntry(tracks.GetNthIndex(i))->SetFont(font);

			/* Update track.
			 */
			tracks.GetNthReference(i) = track;

			/* Add it to the list of modified tracks.
			 */
			modified.Add(track.GetTrackID(), tracks.GetNthIndex(i));

			if (tracks.GetNthIndex(i) == list_files->GetSelectedEntryNumber()) btn_save->Activate();

			btn_saveall->Activate();

			return;
		}
	}
}

/* Called when the "Save" button is clicked.
 * ----
 * Invokes SaveFileTag() for the selected file.
 */
Void BoCA::ChooserFiles::OnSave()
{
	Int	 i = list_files->GetSelectedEntryNumber();

	if (SaveFileTag(tracks.Get(i)) != Success()) return;

	Font	 font = list_files->GetNthEntry(i)->GetFont();

	font.SetColor(Color(0, 0, 0));

	list_files->GetNthEntry(i)->SetFont(font);

	modified.Remove(i);

	btn_save->Deactivate();

	if (modified.Length() == 0) btn_saveall->Deactivate();
}

/* Called when the "Save all" button is clicked.
 * ----
 * Invokes SaveFileTag() for all modified files.
 */
Void BoCA::ChooserFiles::OnSaveAll()
{
	Int	 failed = 0;

	while (modified.Length() - failed > 0)
	{
		Int	 i = modified.GetNthIndex(0 + failed);

		if (SaveFileTag(tracks.Get(i)) != Success()) { failed++; continue; }

		Font	 font = list_files->GetNthEntry(i)->GetFont();

		font.SetColor(Color(0, 0, 0));

		list_files->GetNthEntry(i)->SetFont(font);

		modified.Remove(i);
	}

	if (modified.Get(list_files->GetSelectedEntryNumber()) == NIL) btn_save->Deactivate();

	if (modified.Length() == 0) btn_saveall->Deactivate();
}

Int BoCA::ChooserFiles::SaveFileTag(const Track &track)
{
	Config	*config = Config::Get();

	Int	 error = Error();
	String	 errorString = "Unknown error";

	/* Get tagging mode and type
	 */
	DecoderComponent	*decoder = ChooserFilesUtilities::CreateDecoderComponent(track.origFilename);
	FileFormat		*format = decoder->GetFormats().GetFirst();

	Int	 tagMode = format->GetTagMode();
	String	 tagType = format->GetTagType();

	Registry::Get().DeleteComponent(decoder);

	/* Update tag.
	 */
	if (tagMode != TAG_MODE_NONE)
	{
		String		 taggerID;

		if	(tagType == "ID3v1"	   && config->GetIntValue("Tags", "EnableID3v1", False))       taggerID = "id3v1-tag";
		else if	(tagType == "ID3v2"	   && config->GetIntValue("Tags", "EnableID3v2", True))	       taggerID = "id3v2-tag";
		else if	(tagType == "APEv2"	   && config->GetIntValue("Tags", "EnableAPEv2", True))	       taggerID = "apev2-tag";
		else if (tagType == "MP4Metadata"  && config->GetIntValue("Tags", "EnableMP4Metadata", True))  taggerID = "mp4-tag";
		else if (tagType == "FLACMetadata" && config->GetIntValue("Tags", "EnableFLACMetadata", True)) taggerID = "flac-tag";
		else if (tagType == "WMAMetadata"  && config->GetIntValue("Tags", "EnableWMAMetadata", True))  taggerID = "wma-tag";

		Registry	&boca = AS::Registry::Get();
		TaggerComponent	*tagger = (AS::TaggerComponent *) AS::Registry::Get().CreateComponentByID(taggerID);

		if (tagger != NIL)
		{
			error = tagger->UpdateStreamInfo(track.origFilename, track);
			errorString = tagger->GetErrorString();

			boca.DeleteComponent(tagger);
		}
	}

	/* Check for error.
	 */
	if (error == Error())
	{
		Utilities::ErrorMessage(String(BoCA::I18n::Get()->TranslateString("Unable to update tag: %1\n\nError: %2")).Replace("%1", track.origFilename).Replace("%2", BoCA::I18n::Get()->TranslateString(errorString)));

		return Error();
	}
	
	return Success();
}

Void BoCA::ChooserFiles::GetSupportedFileExtensions()
{
	if (extensions.Length() > 0) return;

	Registry		&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_DECODER) continue;

		const Array<FileFormat *>	&formats = boca.GetComponentFormats(i);

		foreach (FileFormat *format, formats)
		{
			const Array<String>	&extensions = format->GetExtensions();

			foreach (String extension, extensions) this->extensions.Add(extension.ToLower(), extension.ToLower().ComputeCRC32());
		}
	}
}