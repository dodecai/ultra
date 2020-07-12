#pragma once

bool StringSearch(const std::string &strHaystack, const std::string &strNeedle) {
	auto it = std::search(
		strHaystack.begin(), strHaystack.end(),
		strNeedle.begin(),   strNeedle.end(),
		[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
	);
	return (it != strHaystack.end() );
}

class ConsoleView {
	char Buffer[1024];
	std::vector<std::string> Commands;
	std::vector<std::string> Items;
	std::vector<std::string> History;
	int32_t HistoryIndex = -1;

	bool AutoScroll = true;
	bool ScrollToBottom = true;
	ImGuiTextFilter Filter;

public:
	ConsoleView() {
		Clear();
		memset(Buffer, 0, sizeof(Buffer));

		Commands.reserve(32);
		Items.reserve(1024);
		History.reserve(64);

		// CommandList
		Commands.push_back("clear");
		Commands.push_back("help");
		Commands.push_back("history");
		Commands.push_back("test");

		Add("▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬");
		Add("Welcome to Ultra console!");
		Add("▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬");
		Add("Enter 'help' for help and press 'tab' to use text completion.");
	}
	~ConsoleView() {
		Clear();
	}

	void Add(const std::string data) {
		Items.push_back(data);
	}

	static int Callback(ImGuiInputTextCallbackData *data) {
		ConsoleView *console = (ConsoleView *)data->UserData;
		return console->Input(data);
	}
	void Draw(const std::string &title, bool *p_open) {

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertColorRgbaVec(MaterialOcean::Background, 0.92f));
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(title.c_str(), p_open)) {
			ImGui::End();
			ImGui::PopStyleColor();
			return;
		}

		// Here we create a context menu only available from the title bar.
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Close")) *p_open = false;
			if (ImGui::Selectable("Clear")) Clear();
			ImGui::Separator();
			ImGui::Text("Options");
			ImGui::Checkbox("Auto-scroll", &AutoScroll);
			ImGui::Checkbox("Scroll-to-buttom", &ScrollToBottom);
			ImGui::EndPopup();
		}

		// Text Buffer
		ImGui::Separator();

		const float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("ScrollingRegion", { 0, -footerHeight }, false, ImGuiWindowFlags_HorizontalScrollbar);

		if (ImGui::BeginPopupContextWindow()) {
			ImGui::Selectable("Copy");
			ImGui::EndPopup();
		}

		for (auto &it : Items) {
			if (StringSearch(it, "▬")) {
				ImGui::TextColored(ConvertColorRgbaVec(MaterialOcean::Blue), it.c_str());
			} else if (StringSearch(it, "info")) {
				ImGui::TextColored(ConvertColorRgbaVec(MaterialOcean::LightGray), it.c_str());
			} else if (StringSearch(it, "warn")) {
				ImGui::TextColored(ConvertColorRgbaVec(MaterialOcean::LightYellow), it.c_str());
			} else if (StringSearch(it, "error")) {
				ImGui::TextColored(ConvertColorRgbaVec(MaterialOcean::LightRed), it.c_str());
			} else if (StringSearch(it, "fatal")) {
				ImGui::TextColored(ConvertColorRgbaVec(MaterialOcean::Red), it.c_str());
			} else if (StringSearch(it, "debug")) {
				ImGui::TextColored(ConvertColorRgbaVec(MaterialOcean::LightGreen), it.c_str());
			} else if (StringSearch(it, "trace")) {
				ImGui::TextColored(ConvertColorRgbaVec(MaterialOcean::LightMagenta), it.c_str());
			} else {
				ImGui::TextColored(ConvertColorRgbaVec(MaterialOcean::Foreground), it.c_str());
			}

		}

		// Display every line as a separate entry so we can change their color or add custom widgets.
		// If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
		// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
		// to only process visible items. The clipper will automatically measure the height of your first item and then
		// "seek" to display only items in the visible area.
		// To use the clipper we can replace your standard loop:
		//      for (int i = 0; i < Items.Size; i++)
		//   With:
		//      ImGuiListClipper clipper(Items.Size);
		//      while (clipper.Step())
		//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		// - That your items are evenly spaced (same height)
		// - That you have cheap random access to your elements (you can access them given their index,
		//   without processing all the ones before)
		// You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
		// We would need random-access on the post-filtered list.
		// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
		// or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
		// and appending newly elements as they are inserted. This is left as a task to the user until we can manage
		// to improve this example code!
		// If your items are of variable height:
		// - Split them into same height items would be simpler and facilitate random-seeking into your list.
		// - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
		//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));  // Tighten spacing
		//if (copy_to_clipboard)
		//	ImGui::LogToClipboard();
		//for (int i = 0; i < Items.Size; i++) {
		//	const char *item = Items[i];
		//	if (!Filter.PassFilter(item))
		//		continue;

		//	// Normally you would store more information in your item than just a string.
		//	// (e.g. make Items[] an array of structure, store color/type etc.)
		//	ImVec4 color;
		//	bool has_color = false;
		//	if (strstr(item, "[error]")) {
		//		color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
		//		has_color = true;
		//	} else if (strncmp(item, "# ", 2) == 0) {
		//		color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
		//		has_color = true;
		//	}
		//	if (has_color)
		//		ImGui::PushStyleColor(ImGuiCol_Text, color);
		//	ImGui::TextUnformatted(item);
		//	if (has_color)
		//		ImGui::PopStyleColor();
		//}
		//if (copy_to_clipboard) ImGui::LogFinish();

		if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) ImGui::SetScrollHereY(1.0f);
		ScrollToBottom = false;

		//ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();

		// Command Input
		bool reclaim_focus = false;
		ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
		if (ImGui::InputText("Input", Buffer, IM_ARRAYSIZE(Buffer), input_text_flags, &Callback, (void *)this)) {
			char *s = Buffer;
			//Strtrim(s);
			if (s[0]) Execute(s);
			strcpy(s, "");
			reclaim_focus = true;
		}
		ImGui::SetItemDefaultFocus();
		if (reclaim_focus) ImGui::SetKeyboardFocusHere(-1);  // Auto focus previous widget

		// Filter Input
		ImGui::SameLine(ImGui::GetWindowWidth() -224.0f);
		Filter.Draw("Filter", 180);

		ImGui::End();
		ImGui::PopStyleColor();
	}
	void Execute(const std::string &command) {
		if (command == "clear") {
			Clear();
		} else if (command == "help") {
			Add("Commands:");
			for (auto &item : Commands) Add(" - " + item);
		} else if (command == "history") {
			if (History.empty()) {
				Add("History is empty!");
			} else {
				Add("History:");
				for (auto &item : History) Add(item);
			}
			return;
		} else if (command == "test") {
			Add("Default Message");
			Add(Ultra::apptime.GetTimeStamp() + ": [Info] Information Message");
			Add(Ultra::apptime.GetTimeStamp() + ": [Warn] Warning Message");
			Add(Ultra::apptime.GetTimeStamp() + ": [Error] Error Message");
			Add(Ultra::apptime.GetTimeStamp() + ": [Fatal] Critical Message");
			Add(Ultra::apptime.GetTimeStamp() + ": [Debug] Debug Message");
			Add(Ultra::apptime.GetTimeStamp() + ": [Trace] Trace Message");
		} else {
			std::string &message = "unknown command '" + command + "'";
			Add(message);
		}

		// Search in history for command and push it back
		History.push_back(command.c_str());
		HistoryIndex++;

		// On command input, we scroll to bottom even if AutoScroll==false
		ScrollToBottom = true;
	}
	int Input(ImGuiInputTextCallbackData *data) {
		Items.push_back(data->Buf);
		return 0;
	}

	// Commands
	void Clear() {
		Items.clear();
	}
	//void Insert(const std::string &format, ...) IM_FMTARGS(2) {
	//	//// FIXME-OPT
	//	//char buf[1024];
	//	//va_list args;
	//	//va_start(args, fmt);
	//	//vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	//	//buf[IM_ARRAYSIZE(buf) - 1] = 0;
	//	//va_end(args);
	//	//Items.push_back(Strdup(buf));
	//}
};

struct AppConsole {
	char InputBuf[256];
	ImVector<char *> Items;
	ImVector<const char *> Commands;
	ImVector<char *> History;
	int HistoryPos;	 // -1: new line, 0..History.Size-1 browsing history.
	ImGuiTextFilter Filter;
	bool AutoScroll;
	bool ScrollToBottom;

	AppConsole() {
		ClearLog();
		memset(InputBuf, 0, sizeof(InputBuf));
		HistoryPos = -1;

		// "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
		Commands.push_back("help");
		Commands.push_back("history");
		Commands.push_back("clear");
		Commands.push_back("classify");
		AutoScroll = true;
		ScrollToBottom = false;
		AddLog("Welcome to Dear ImGui!");
	}
	~AppConsole() {
		ClearLog();
		for (int i = 0; i < History.Size; i++)
			free(History[i]);
	}

	// Portable helpers
	static int Stricmp(const char *s1, const char *s2) {
		int d;
		while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
			s1++;
			s2++;
		}
		return d;
	}
	static int Strnicmp(const char *s1, const char *s2, int n) {
		int d = 0;
		while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
			s1++;
			s2++;
			n--;
		}
		return d;
	}
	static char *Strdup(const char *s) {
		size_t len = strlen(s) + 1;
		void *buf = malloc(len);
		IM_ASSERT(buf);
		return (char *)memcpy(buf, (const void *)s, len);
	}
	static void Strtrim(char *s) {
		char *str_end = s + strlen(s);
		while (str_end > s && str_end[-1] == ' ') str_end--;
		*str_end = 0;
	}

	void ClearLog() {
		for (int i = 0; i < Items.Size; i++)
			free(Items[i]);
		Items.clear();
	}

	void AddLog(const char *fmt, ...) IM_FMTARGS(2) {
		// FIXME-OPT
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		Items.push_back(Strdup(buf));
	}

	void Draw(const char *title, bool *p_open) {
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(title, p_open)) {
			ImGui::End();
			return;
		}

		// As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
		// So e.g. IsItemHovered() will return true when hovering the title bar.
		// Here we create a context menu only available from the title bar.
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Close Console"))
				*p_open = false;
			ImGui::EndPopup();
		}

		//ImGui::TextWrapped(
		//	"This example implements a console with basic coloring, completion and history. A more elaborate "
		//	"implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
		ImGui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");

		// TODO: display items starting from the bottom

		if (ImGui::SmallButton("Add Dummy Text")) {
			AddLog("%d some text", Items.Size);
			AddLog("some more text");
			AddLog("display very important message here!");
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("Add Dummy Error")) { AddLog("[error] something went wrong"); }
		ImGui::SameLine();
		if (ImGui::SmallButton("Clear")) { ClearLog(); }
		ImGui::SameLine();
		bool copy_to_clipboard = ImGui::SmallButton("Copy");
		//static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

		ImGui::Separator();

		// Options menu
		if (ImGui::BeginPopup("Options")) {
			ImGui::Checkbox("Auto-scroll", &AutoScroll);
			ImGui::EndPopup();
		}

		// Options, Filter
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
		ImGui::SameLine();
		Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
		ImGui::Separator();

		// Reserve enough left-over height for 1 separator + 1 input text
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::Selectable("Clear")) ClearLog();
			ImGui::EndPopup();
		}

		// Display every line as a separate entry so we can change their color or add custom widgets.
		// If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
		// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
		// to only process visible items. The clipper will automatically measure the height of your first item and then
		// "seek" to display only items in the visible area.
		// To use the clipper we can replace your standard loop:
		//      for (int i = 0; i < Items.Size; i++)
		//   With:
		//      ImGuiListClipper clipper(Items.Size);
		//      while (clipper.Step())
		//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		// - That your items are evenly spaced (same height)
		// - That you have cheap random access to your elements (you can access them given their index,
		//   without processing all the ones before)
		// You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
		// We would need random-access on the post-filtered list.
		// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
		// or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
		// and appending newly elements as they are inserted. This is left as a task to the user until we can manage
		// to improve this example code!
		// If your items are of variable height:
		// - Split them into same height items would be simpler and facilitate random-seeking into your list.
		// - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));  // Tighten spacing
		if (copy_to_clipboard)
			ImGui::LogToClipboard();
		for (int i = 0; i < Items.Size; i++) {
			const char *item = Items[i];
			if (!Filter.PassFilter(item))
				continue;

			// Normally you would store more information in your item than just a string.
			// (e.g. make Items[] an array of structure, store color/type etc.)
			ImVec4 color;
			bool has_color = false;
			if (strstr(item, "[error]")) {
				color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
				has_color = true;
			} else if (strncmp(item, "# ", 2) == 0) {
				color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
				has_color = true;
			}
			if (has_color)
				ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted(item);
			if (has_color)
				ImGui::PopStyleColor();
		}
		if (copy_to_clipboard)
			ImGui::LogFinish();

		if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
			ImGui::SetScrollHereY(1.0f);
		ScrollToBottom = false;

		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();

		// Command-line
		bool reclaim_focus = false;
		ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
		if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void *)this)) {
			char *s = InputBuf;
			Strtrim(s);
			if (s[0])
				ExecCommand(s);
			strcpy(s, "");
			reclaim_focus = true;
		}

		// Auto-focus on window apparition
		ImGui::SetItemDefaultFocus();
		if (reclaim_focus)
			ImGui::SetKeyboardFocusHere(-1);  // Auto focus previous widget

		ImGui::End();
	}

	void ExecCommand(const char *command_line) {
		AddLog("# %s\n", command_line);

		// Insert into history. First find match and delete it so it can be pushed to the back.
		// This isn't trying to be smart or optimal.
		HistoryPos = -1;
		for (int i = History.Size - 1; i >= 0; i--)
			if (Stricmp(History[i], command_line) == 0) {
				free(History[i]);
				History.erase(History.begin() + i);
				break;
			}
		History.push_back(Strdup(command_line));

		// Process command
		if (Stricmp(command_line, "CLEAR") == 0) {
			ClearLog();
		} else if (Stricmp(command_line, "HELP") == 0) {
			AddLog("Commands:");
			for (int i = 0; i < Commands.Size; i++)
				AddLog("- %s", Commands[i]);
		} else if (Stricmp(command_line, "HISTORY") == 0) {
			int first = History.Size - 10;
			for (int i = first > 0 ? first : 0; i < History.Size; i++)
				AddLog("%3d: %s\n", i, History[i]);
		} else {
			AddLog("Unknown command: '%s'\n", command_line);
		}

		// On command input, we scroll to bottom even if AutoScroll==false
		ScrollToBottom = true;
	}

	// In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
	static int TextEditCallbackStub(ImGuiInputTextCallbackData *data) {
		AppConsole *console = (AppConsole *)data->UserData;
		return console->TextEditCallback(data);
	}

	int TextEditCallback(ImGuiInputTextCallbackData *data) {
		//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
		switch (data->EventFlag) {
			case ImGuiInputTextFlags_CallbackCompletion: {
				// Example of TEXT COMPLETION

				// Locate beginning of current word
				const char *word_end = data->Buf + data->CursorPos;
				const char *word_start = word_end;
				while (word_start > data->Buf) {
					const char c = word_start[-1];
					if (c == ' ' || c == '\t' || c == ',' || c == ';')
						break;
					word_start--;
				}

				// Build a list of candidates
				ImVector<const char *> candidates;
				for (int i = 0; i < Commands.Size; i++)
					if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
						candidates.push_back(Commands[i]);

				if (candidates.Size == 0) {
					// No match
					AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
				} else if (candidates.Size == 1) {
					// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
					data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0]);
					data->InsertChars(data->CursorPos, " ");
				} else {
					// Multiple matches. Complete as much as we can..
					// So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
					int match_len = (int)(word_end - word_start);
					for (;;) {
						int c = 0;
						bool all_candidates_matches = true;
						for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
							if (i == 0)
								c = toupper(candidates[i][match_len]);
							else if (c == 0 || c != toupper(candidates[i][match_len]))
								all_candidates_matches = false;
						if (!all_candidates_matches)
							break;
						match_len++;
					}

					if (match_len > 0) {
						data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
						data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
					}

					// List matches
					AddLog("Possible matches:\n");
					for (int i = 0; i < candidates.Size; i++)
						AddLog("- %s\n", candidates[i]);
				}

				break;
			}
			case ImGuiInputTextFlags_CallbackHistory: {
				// Example of HISTORY
				const int prev_history_pos = HistoryPos;
				if (data->EventKey == ImGuiKey_UpArrow) {
					if (HistoryPos == -1)
						HistoryPos = History.Size - 1;
					else if (HistoryPos > 0)
						HistoryPos--;
				} else if (data->EventKey == ImGuiKey_DownArrow) {
					if (HistoryPos != -1)
						if (++HistoryPos >= History.Size)
							HistoryPos = -1;
				}

				// A better implementation would preserve the data on the current input line along with cursor position.
				if (prev_history_pos != HistoryPos) {
					const char *history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, history_str);
				}
			}
		}
		return 0;
	}
};

static void ShowExampleAppConsole(bool *p_open) {
	static ConsoleView console;
	console.Draw("Console", p_open);
}
