 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2013 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "pls_list.h"

using namespace smooth::IO;

const String &BoCA::PLSPlaylist::GetComponentSpecs()
{
	static String	 componentSpecs = "		\
							\
	  <?xml version=\"1.0\" encoding=\"UTF-8\"?>	\
	  <component>					\
	    <name>PLS Playlist</name>			\
	    <version>1.0</version>			\
	    <id>pls-playlist</id>			\
	    <type>playlist</type>			\
	    <format>					\
	      <name>PLS Playlist</name>			\
	      <extension>pls</extension>		\
	    </format>					\
	  </component>					\
							\
	";

	return componentSpecs;
}

BoCA::PLSPlaylist::PLSPlaylist()
{
}

BoCA::PLSPlaylist::~PLSPlaylist()
{
}

Bool BoCA::PLSPlaylist::CanOpenFile(const String &file)
{
	InStream	 in(STREAM_FILE, file, IS_READ);

	while (in.GetPos() < in.Size())
	{
		String	 line = in.InputLine();

		if	(line == "[playlist]")		       return True;
		else if (line != NIL && !line.StartsWith(";")) return False;
	}

	return False;
}

const Array<BoCA::Track> &BoCA::PLSPlaylist::ReadPlaylist(const String &file)
{
	String		 format = String::SetInputFormat("ISO-8859-1");
	InStream	 in(STREAM_FILE, file, IS_READ);

	while (in.GetPos() < in.Size())
	{
		String	 line = in.InputLine();

		if (line.StartsWith("File"))
		{
			Track	 track;

			track.origFilename = line.Tail(line.Length() - line.Find("=") - 1);

#ifdef __WIN32__
			if (track.origFilename[1] != ':' && !track.origFilename.StartsWith("\\\\") && !track.origFilename.Contains("://"))
#else
			if (!track.origFilename.StartsWith(Directory::GetDirectoryDelimiter()) && !track.origFilename.StartsWith("~") && !track.origFilename.Contains("://"))
#endif
			{
				track.origFilename = String(File(file).GetFilePath()).Append(Directory::GetDirectoryDelimiter()).Append(track.origFilename);
			}

			trackList.Add(track);
		}
	}

	in.Close();

	String::SetInputFormat(format);

	return trackList;
}

Error BoCA::PLSPlaylist::WritePlaylist(const String &file)
{
	if (trackList.Length() == 0) return Error();

	String		 format = String::SetOutputFormat("ISO-8859-1");

	String		 actualFile = Utilities::CreateDirectoryForFile(file);
	OutStream	 out(STREAM_FILE, actualFile, OS_REPLACE);

	if (out.GetLastError() != IO_ERROR_OK)
	{
		Utilities::ErrorMessage("Could not create playlist file:\n\n%1", actualFile);

		return Error();
	}

	BoCA::I18n	*i18n = BoCA::I18n::Get();

	out.OutputLine("[playlist]");
	out.OutputLine("Version=2");
	out.OutputLine(NIL);
	out.OutputLine(String("NumberOfEntries=").Append(String::FromInt(trackList.Length())));

	for (Int i = 0; i < trackList.Length(); i++)
	{
		const Track	&track = trackList.GetNth(i);

		/* Special handling for CD tracks on Windows.
		 */
		String		 fileName = Utilities::GetCDTrackFileName(track);

		/* Write info to file.
		 */
		const Info	&info = track.GetInfo();

		out.OutputLine(NIL);
		out.OutputLine(String("File").Append(String::FromInt(i + 1)).Append("=").Append(Utilities::GetRelativeFileName(fileName, actualFile)));
		out.OutputLine(String("Title").Append(String::FromInt(i + 1)).Append("=").Append(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(info.title.Length() > 0 ? info.title : i18n->TranslateString("unknown title")));
		out.OutputLine(String("Length").Append(String::FromInt(i + 1)).Append("=").Append(String::FromInt(track.length == -1 ? -1 : Math::Round((Float) track.length / track.GetFormat().rate))));
	}

	out.Close();

	String::SetOutputFormat(format);

	return Success();
}