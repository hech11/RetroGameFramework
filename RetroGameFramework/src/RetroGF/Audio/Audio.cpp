#include "RGFpch.h"
#include "Audio.h"

#include "AudioBuffer.h"


#include "RetroGF/Debug/Instrumentor.h"

namespace RGF {


	struct AudioData {
		Ref<AudioContext> Context;
		Ref<AudioListener> Listener;

		std::map<std::string, Ref<AudioBuffer>> AudioBuffers;
	};

	static AudioData s_Data;

	void Audio::Init() {
		RGF_PROFILE_FUNCTION();
		s_Data.Context = CreateRef<AudioContext>();
		s_Data.Context->Init();

		s_Data.Listener = CreateRef<AudioListener>();


	}
	void Audio::Shutdown() {
		RGF_PROFILE_FUNCTION();
		s_Data.Context->Close();
	}


	Ref<AudioSource> Audio::CreateAudioSource(const std::string& filepath, bool shouldLoop /*= false*/, bool streaming /*= false*/) {
		RGF_PROFILE_FUNCTION();

		Ref<AudioSource> source = CreateRef<AudioSource>();
		source->SetLooping(shouldLoop);

		if (s_Data.AudioBuffers.find(filepath) != s_Data.AudioBuffers.end()) {
			RGF_PROFILE_SCOPE("Audio::CreateAudioSource::.AudioBuffers.find(filepath) != s_Data.AudioBuffers.end()");
			source->SetBufferData(s_Data.AudioBuffers[filepath]);
			return source;
		}

		AudioFormatSpec specs = AudioFormatSpec::LoadAudioData(filepath);
		Ref<AudioBuffer> buffer = CreateRef<AudioBuffer>(specs);

		s_Data.AudioBuffers[filepath] = buffer;
		source->SetBufferData(buffer);

		return source;
	}

	
	

}