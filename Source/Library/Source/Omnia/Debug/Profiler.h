
// Scoped Timer as Profiler
//template<typename L>
//class ScopedTimer {
//	bool Stopped;
//	const char *Name;
//	std::chrono::time_point<std::chrono::steady_clock> StartTimePoint;
//	L Function;
//
//public:
//	ScopedTimer(const char *name, L &&function): Name{ name }, Stopped{ false }, Function{ function } {
//		StartTimePoint = std::chrono::high_resolution_clock::now();
//	}
//	~ScopedTimer() { if (!Stopped) { Stop(); } };
//
//	void Stop() {
//		auto stopTimePoint = std::chrono::high_resolution_clock::now();
//
//		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(StartTimePoint).time_since_epoch().count();
//		auto stop = std::chrono::time_point_cast<std::chrono::microseconds>(stopTimePoint).time_since_epoch().count();
//
//		Stopped = true;
//
//		auto duration = (stop - start) * 0.001f;
//		Function({ Name, duration });
//	}
//
//};
//
//struct ProfileResult {
//	const char *Name;
//	float Time;
//};
//
//#define PROFILE_SCOPE(name) ScopedTimer timer##__LINE__(name, [&](ProfileResult result) { ProfileResults.push_back(result); })
//
//std::vector<ProfileResult> ProfileResults;
//PROFILE_SCOPE("TestLayer2D::Update");
//PROFILE_SCOPE("TestLayer2D::Update::Camera");
//PROFILE_SCOPE("TestLayer2D::Update::RendererPrepare");
//PROFILE_SCOPE("TestLayer2D::Update::RendererDraw");
//ImGui::Begin("Statistics");
//for (auto &result : ProfileResults) {
//	char label[64];
//	strcpy(label, "%.3fms ");
//	strcat(label, result.Name);
//	ImGui::Text(label, result.Time);
//}
//ProfileResults.clear();
