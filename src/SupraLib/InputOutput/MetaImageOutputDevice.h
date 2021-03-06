// ================================================================================================
// 
// If not explicitly stated: Copyright (C) 2011-2016, all rights reserved,
//      Christoph Hennersperger 
//		EmaiL christoph.hennersperger@tum.de
//      Chair for Computer Aided Medical Procedures
//      Technische Universität München
//      Boltzmannstr. 3, 85748 Garching b. München, Germany
//	and
//		Rüdiger Göbl
//		Email r.goebl@tum.de
//
// ================================================================================================

#ifndef __METAIMAGEOUTPUTDEVICE_H__
#define __METAIMAGEOUTPUTDEVICE_H__

#ifdef HAVE_DEVICE_METAIMAGE_OUTPUT

#include "AbstractOutput.h"

namespace supra
{
	//forward declarations
	class MhdSequenceWriter;
	class USImage;
	class USRawData;

	class MetaImageOutputDevice : public AbstractOutput
	{
	public:
		MetaImageOutputDevice(tbb::flow::graph& graph, const std::string & nodeID, bool queueing);
		~MetaImageOutputDevice();

		//Functions to be overwritten
	public:
		virtual void initializeOutput();
		virtual bool ready();
		//Needs to be thread safe
		virtual void startSequence();
		//Needs to be thread safe
		virtual void stopSequence();
	protected:
		virtual void startOutput();
		//Needs to be thread safe
		virtual void stopOutput();
		//Needs to be thread safe
		virtual void configurationDone();

		virtual void writeData(std::shared_ptr<RecordObject> data);

	private:
		void initializeSequence();
		void addData(std::shared_ptr<const RecordObject> data);
		std::pair<bool, size_t> addSyncRecord(std::shared_ptr<const RecordObject> _syncMessage);
		std::pair<bool, size_t> addImage(std::shared_ptr<const RecordObject> imageData);
		std::pair<bool, size_t> addUSRawData(std::shared_ptr<const RecordObject> _rawData);
		void addTracking(std::shared_ptr<const RecordObject> trackingData, size_t frameNum);

		template <typename ElementType>
		std::pair<bool, size_t> addImageTemplated(std::shared_ptr<const USImage> imageData);
		template <typename ElementType>
		std::pair<bool, size_t> addUSRawDataTemplated(std::shared_ptr <const USRawData> rawData);

		MhdSequenceWriter* m_pWriter;

		bool m_isReady;
		bool m_createSequences;
		bool m_active;
		std::atomic_bool m_isRecording;
		size_t m_sequencesWritten;

		// maximum number of individual elements (frames,sequences,trackingSets) to be written
		size_t m_maxElementNumber;
		size_t m_lastElementNumber;

		std::mutex m_writerMutex;

		std::string m_filename;
	};
}

#endif //!HAVE_DEVICE_METAIMAGE_OUTPUT

#endif //!__METAIMAGEOUTPUTDEVICE_H__
