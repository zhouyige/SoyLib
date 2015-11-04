#include "SoyMedia.h"
#include "SortArray.h"



std::map<SoyMediaFormat::Type,std::string> SoyMediaFormat::EnumMap =
{
	{ SoyMediaFormat::Invalid,			"Invalid" },
	{ SoyMediaFormat::H264_8,			"H264_8" },
	{ SoyMediaFormat::H264_16,			"H264_16" },
	{ SoyMediaFormat::H264_32,			"H264_32" },
	{ SoyMediaFormat::H264_ES,			"H264_ES" },
	{ SoyMediaFormat::H264_SPS_ES,		"H264_SPS_ES" },
	{ SoyMediaFormat::H264_PPS_ES,		"H264_PPS_ES" },
	{ SoyMediaFormat::Mpeg2TS,			"Mpeg2TS" },
	{ SoyMediaFormat::Mpeg2,			"Mpeg2" },
	{ SoyMediaFormat::Audio,			"audio" },
	{ SoyMediaFormat::Wave,				"wave" },
	{ SoyMediaFormat::Aac,				"aac" },
	{ SoyMediaFormat::Text,				"text" },
	{ SoyMediaFormat::Subtitle,			"subtitle" },
	{ SoyMediaFormat::ClosedCaption,	"closedcaption" },
	{ SoyMediaFormat::Timecode,			"timecode" },
	{ SoyMediaFormat::MetaData,			"metadata" },
	{ SoyMediaFormat::Muxed,			"muxed" },
	
	
	{ SoyMediaFormat::Greyscale,		"Greyscale" },
	{ SoyMediaFormat::GreyscaleAlpha,	"GreyscaleAlpha" },
	{ SoyMediaFormat::RGB,				"RGB" },
	{ SoyMediaFormat::RGBA,				"RGBA" },
	{ SoyMediaFormat::BGRA,				"BGRA" },
	{ SoyMediaFormat::BGR,				"BGR" },
	{ SoyMediaFormat::KinectDepth,		"KinectDepth" },
	{ SoyMediaFormat::FreenectDepth10bit,	"FreenectDepth10bit" },
	{ SoyMediaFormat::FreenectDepth11bit,	"FreenectDepth11bit" },
	{ SoyMediaFormat::FreenectDepthmm,		"FreenectDepthmm" },
	{ SoyMediaFormat::LumaFull,			"LumaFull" },
	{ SoyMediaFormat::LumaVideo,		"LumaVideo" },
	{ SoyMediaFormat::Yuv_8_88_Full,	"Yuv_8_88_Full" },
	{ SoyMediaFormat::Yuv_8_88_Video,	"Yuv_8_88_Video" },
	{ SoyMediaFormat::Yuv_8_4_4_Full,	"Yuv_8_4_4_Full" },
	{ SoyMediaFormat::ChromaUV_4_4,		"ChromaUV_4_4" },
	{ SoyMediaFormat::ChromaUV_88,		"ChromaUV_88" },
};



std::ostream& operator<<(std::ostream& out,const TStreamMeta& in)
{
	out << "mCodec=" << in.mCodec << ", ";
	out << "Mime=" << in.GetMime() << ", ";
	out << "mDescription=" << in.mDescription << ", ";
	out << "mCompressed=" << in.mCompressed << ", ";
	out << "mFramesPerSecond=" << in.mFramesPerSecond << ", ";
	out << "mFramesPerSecond=" << in.mFramesPerSecond << ", ";
	out << "mDuration=" << in.mDuration << ", ";
	out << "mEncodingBitRate=" << in.mEncodingBitRate << ", ";
	out << "mStreamIndex=" << in.mStreamIndex << ", ";
	out << "mMediaTypeIndex=" << in.mMediaTypeIndex << ", ";
	out << "mPixelMeta=" << in.mPixelMeta << ", ";
	out << "mInterlaced=" << in.mInterlaced << ", ";
	out << "mVideoClockWiseRotationDegrees=" << in.mVideoClockWiseRotationDegrees << ", ";
	out << "m3DVideo=" << in.m3DVideo << ", ";
	//out << "mYuvMatrix=" << in.mYuvMatrix << ", ";
	out << "mDrmProtected=" << in.mDrmProtected << ", ";
	out << "mMaxKeyframeSpacing=" << in.mMaxKeyframeSpacing << ", ";
	out << "mAverageBitRate=" << in.mAverageBitRate << ", ";
	out << "mChannelCount=" << in.mChannelCount << ", ";
	
	return out;
}


std::ostream& operator<<(std::ostream& out,const TMediaPacket& in)
{
	out << "MediaPacket: ";
	out << " Codec=" << in.mMeta.mCodec;
	out << " mTimecode=" << in.mTimecode;
	out << " mIsKeyFrame=" << in.mIsKeyFrame;
	out << " mEncrypted=" << in.mEncrypted;
	out << " Size=" << in.mData.GetDataSize();
	return out;
}


SoyPixelsFormat::Type SoyMediaFormat::GetPixelFormat(SoyMediaFormat::Type MediaFormat)
{
	if ( MediaFormat == SoyMediaFormat::Invalid )
		return SoyPixelsFormat::Invalid;
	
	if ( MediaFormat >= SoyMediaFormat::NotPixels )
		return SoyPixelsFormat::Invalid;
	
	return static_cast<SoyPixelsFormat::Type>( MediaFormat );
}



bool SoyMediaFormat::IsVideo(SoyMediaFormat::Type Format)
{
	if ( IsPixels(Format) )
		return true;
	if ( IsH264(Format) )
		return true;
	
	switch ( Format )
	{
		case SoyMediaFormat::Mpeg2TS:		return true;
		case SoyMediaFormat::Mpeg2:			return true;
		default:							return false;
	}
}

bool SoyMediaFormat::IsH264(SoyMediaFormat::Type Format)
{
	switch ( Format )
	{
		case SoyMediaFormat::H264_8:		return true;
		case SoyMediaFormat::H264_16:		return true;
		case SoyMediaFormat::H264_32:		return true;
		case SoyMediaFormat::H264_ES:		return true;
		case SoyMediaFormat::H264_SPS_ES:	return true;
		case SoyMediaFormat::H264_PPS_ES:	return true;
		default:							return false;
	}
}

bool SoyMediaFormat::IsAudio(SoyMediaFormat::Type Format)
{
	switch ( Format )
	{
		case SoyMediaFormat::Audio:	return true;
		default:					return false;
	}
}

std::string SoyMediaFormat::ToMime(SoyMediaFormat::Type Format)
{
	switch ( Format )
	{
		case SoyMediaFormat::H264_8:		return "video/avc";
		case SoyMediaFormat::H264_16:		return "video/avc";
		case SoyMediaFormat::H264_32:		return "video/avc";
		case SoyMediaFormat::H264_ES:		return "video/avc";		//	find the proper version of this
		case SoyMediaFormat::H264_PPS_ES:	return "video/avc";		//	find the proper version of this
		case SoyMediaFormat::H264_SPS_ES:	return "video/avc";		//	find the proper version of this
			
		case SoyMediaFormat::Mpeg2TS:	return "video/ts";		//	find the proper version of this
		case SoyMediaFormat::Mpeg2:		return "video/mpeg2";	//	find the proper version of this
	
		case SoyMediaFormat::Wave:		return "audio/wave";
			
		//	gr: change this to handle multiple mime types per format
#if defined(TARGET_ANDROID)
		case SoyMediaFormat::Aac:		return "audio/mp4a-latm";
#else
		case SoyMediaFormat::Aac:		return "audio/x-aac";
#endif
			
		default:						return "invalid/invalid";
	}
	
}

SoyMediaFormat::Type SoyMediaFormat::FromMime(const std::string& Mime)
{
	if ( Mime == ToMime( SoyMediaFormat::H264_8 ) )			return SoyMediaFormat::H264_8;
	if ( Mime == ToMime( SoyMediaFormat::H264_16 ) )		return SoyMediaFormat::H264_16;
	if ( Mime == ToMime( SoyMediaFormat::H264_32 ) )		return SoyMediaFormat::H264_32;
	if ( Mime == ToMime( SoyMediaFormat::H264_ES ) )		return SoyMediaFormat::H264_ES;
	if ( Mime == ToMime( SoyMediaFormat::H264_PPS_ES ) )	return SoyMediaFormat::H264_PPS_ES;
	if ( Mime == ToMime( SoyMediaFormat::H264_SPS_ES ) )	return SoyMediaFormat::H264_SPS_ES;
	if ( Mime == ToMime( SoyMediaFormat::Mpeg2TS ) )		return SoyMediaFormat::Mpeg2TS;
	if ( Mime == ToMime( SoyMediaFormat::Mpeg2 ) )			return SoyMediaFormat::Mpeg2;
	if ( Mime == ToMime( SoyMediaFormat::Wave ) )			return SoyMediaFormat::Wave;
	if ( Mime == ToMime( SoyMediaFormat::Aac ) )			return SoyMediaFormat::Aac;
	
	std::Debug << "Unknown mime type: " << Mime << std::endl;
	return SoyMediaFormat::Invalid;
}

bool SoyMediaFormat::IsH264Fourcc(uint32 Fourcc)
{
	switch ( Fourcc )
	{
		case 'avc1':
		case '1cva':
			return true;
		default:
			return false;
	};
}


SoyMediaFormat::Type SoyMediaFormat::FromFourcc(uint32 Fourcc,int H264LengthSize)
{
	//	gr: handle reverse here automatically?
	switch ( Fourcc )
	{
		case 'avc1':
		case '1cva':
			if ( H264LengthSize == 0 )
				return SoyMediaFormat::H264_ES;
			if ( H264LengthSize == 1 )
				return SoyMediaFormat::H264_8;
			if ( H264LengthSize == 2 )
				return SoyMediaFormat::H264_16;
			if ( H264LengthSize == 4 )
				return SoyMediaFormat::H264_32;
			
		case 'aac ':	return SoyMediaFormat::Aac;
		case ' caa':	return SoyMediaFormat::Aac;
	}
	
	std::Debug << "Unknown fourcc type: " << Soy::FourCCToString(Fourcc) << std::endl;
	return SoyMediaFormat::Invalid;
}






TMediaDecoder::TMediaDecoder(const std::string& ThreadName,std::shared_ptr<TMediaPacketBuffer>& InputBuffer,std::shared_ptr<TPixelBufferManagerBase> OutputBuffer) :
	SoyWorkerThread	( ThreadName, SoyWorkerWaitMode::Wake ),
	mInput			( InputBuffer ),
	mOutput			( OutputBuffer )
{
	//	wake when new packets arrive
	mOnNewPacketListener = WakeOnEvent( mInput->mOnNewPacket );
}

TMediaDecoder::~TMediaDecoder()
{
	if ( mInput )
	{
		mInput->mOnNewPacket.RemoveListener( mOnNewPacketListener );
	}
	WaitToFinish();
}

bool TMediaDecoder::CanSleep()
{
	//	don't sleep when there's work to do!
	if ( mInput )
	{
		if ( mInput->HasPackets() )
			return false;
	}
	
	return true;
}

bool TMediaDecoder::Iteration()
{
	bool FatalCleared = false;
	
	try
	{
		Soy::Assert( mInput!=nullptr, "Input missing");
		
		//	pop next packet
		auto Packet = mInput->PopPacket();
		if ( Packet )
		{
			//std::Debug << "Encoder Processing packet " << Packet->mTimecode << "(pts) " << Packet->mDecodeTimecode << "(dts)" << std::endl;
			if ( !ProcessPacket( *Packet ) )
			{
				std::Debug << "Returned decoder input packet (" << *Packet << ") back to buffer" << std::endl;
				//	gr: only do this for important frames?
				mInput->UnPopPacket( Packet );
			}
		}
	}
	catch (std::exception& e)
	{
		//	gr: unpop packet?
		
		std::Debug << "Error processing input packet " << e.what() << std::endl;
		if ( !FatalCleared )
			mFatalError.clear();
		FatalCleared = true;
		mFatalError << "Error processing input packet " << e.what();
	}
	
	
	//	read outputs
	try
	{
		if ( mOutput )
		{
			ProcessOutputPacket( *mOutput );
		}
	}
	catch (std::exception& e)
	{
		std::Debug << "Error processing output packet " << e.what() << std::endl;
		if ( !FatalCleared )
			mFatalError.clear();
		FatalCleared = true;
		mFatalError << "Error processing output packet " << e.what();
	}
	
	
	return true;
}

TPixelBufferManagerBase& TMediaDecoder::GetPixelBufferManager()
{
	Soy::Assert( mOutput != nullptr, "MediaEncoder missing pixel buffer" );
	return *mOutput;
}





std::shared_ptr<TMediaPacket> TMediaPacketBuffer::PopPacket()
{
	if ( mPackets.IsEmpty() )
		return nullptr;
	
	std::lock_guard<std::mutex> Lock( mPacketsLock );
	//	todo: options here so we can get the next packet we need
	//		where we skip over all frames until the prev-to-Time keyframe
	if ( mPackets.IsEmpty() )
		return nullptr;
	
	//if ( mPackets[0]->mTimecode > Time )
	//	return nullptr;
	
	return mPackets.PopAt(0);
}


void TMediaPacketBuffer::UnPopPacket(std::shared_ptr<TMediaPacket> Packet)
{
	std::lock_guard<std::mutex> Lock( mPacketsLock );
	
	//	gr: no sorted insert, assuming this was the last packet popped (assuming we onlyhave one thing popping packets from this buffer)
	//		so go straight back to the start
	GetArrayBridge(mPackets).InsertAt( 0, Packet );
}

void TMediaPacketBuffer::PushPacket(std::shared_ptr<TMediaPacket> Packet,std::function<bool()> Block)
{
	Soy::Assert( Packet != nullptr, "Packet expected");
	
	//	gr: maybe needs to be atomic?
	while ( mPackets.GetSize() >= mMaxBufferSize )
	{
		if ( !Block() )
		{
			std::Debug << "MediaPacketBuffer buffer full " << mPackets.GetSize() << "/" << mMaxBufferSize << ", dropped packet: " << *Packet << std::endl;
			return;
		}
	}
	
	{
		std::lock_guard<std::mutex> Lock( mPacketsLock );
		
		//	if no decode order, push as they come in...
		//	gr: handle mixes of these... eg. one stream has decode order and one doesnt...
		//	gr: I believe SortArray may push new packets to the start of the array if compare==0 all the way
		if ( Packet->mDecodeTimecode.IsValid() )
		{
			//	sort by decode order
			auto SortPackets = [](const std::shared_ptr<TMediaPacket>& a,const std::shared_ptr<TMediaPacket>& b)
			{
				auto Timea = a->mDecodeTimecode;
				auto Timeb = b->mDecodeTimecode;
				
				if ( Timea < Timeb )
					return -1;
				if ( Timea > Timeb )
					return 1;
				
				return 0;
			};
			
			SortArrayLambda<std::shared_ptr<TMediaPacket>> SortedPackets( GetArrayBridge(mPackets), SortPackets );
			SortedPackets.Push( Packet );
		}
		else
		{
			mPackets.PushBack( Packet );
		}
		
		//std::Debug << "Pushed intput packet to buffer " << *Packet << std::endl;
	}
	mOnNewPacket.OnTriggered( Packet );
}




TMediaExtractor::TMediaExtractor(const std::string& ThreadName) :
SoyWorkerThread		( ThreadName, SoyWorkerWaitMode::Wake )
{
	//	todo: allocate per-stream
	mBuffer.reset( new TMediaPacketBuffer );
}

void TMediaExtractor::Seek(SoyTime Time)
{
	//	update the target seek time
	Soy::Assert( Time >= mSeekTime, "Can't currently handle seeking backwards");
	
	//	update the target time and wake up thread in case we need to read frames
	mSeekTime = Time;
	Wake();
}

void TMediaExtractor::OnEof()
{
	//	stop the thread?
	//std::Debug << "Media extractor EOF" << std::endl;
}

void TMediaExtractor::OnError(const std::string& Error)
{
	//	set fatal error, stop thread?
	std::Debug << "Media extractor OnError(" << Error << ")" << std::endl;
	mFatalError = Error;
}

bool TMediaExtractor::Iteration()
{
	//	tell extractor to read more frames
	//	stall thread if buffer is full, drop frames etc
	try
	{
		auto Loop = [this]
		{
			return IsWorking();
		};
		ReadPacketsUntil( mSeekTime, Loop );
	}
	catch (std::exception& e)
	{
		OnError( e.what() );
	}
	
	return true;
}


void TMediaExtractor::ReadPacketsUntil(SoyTime Time,std::function<bool()> While)
{
	while ( While() )
	{
		try
		{
			auto NextPacket = ReadNextPacket();
			
			//	no packet, error? try again
			if ( !NextPacket )
				continue;
			
			//	temp
			if ( NextPacket->mEof )
				return;
			
			//	save what we read
			Soy::Assert( mBuffer!=nullptr, "Buffer expected" );
			
			//	block thread unless it's stopped
			auto Block = [this]()
			{
				return IsWorking();
			};
			mBuffer->PushPacket( NextPacket, Block );
			
			if ( NextPacket->mEof )
				OnEof();
			
			//	passed the time we were reading until, abort current loop
			if ( NextPacket->mTimecode >= Time )
				return;
		}
		catch ( std::exception& e)
		{
			std::stringstream Error;
			Error << "Error extracting next packet " << e.what();
			OnError( Error.str() );
			return;
		}
	}
}


TStreamMeta TMediaExtractor::GetStream(size_t Index)
{
	Array<TStreamMeta> Streams;
	GetStreams( GetArrayBridge(Streams) );
	auto TotalStreamCount = Streams.GetSize();
	
	if ( Index >= Streams.GetSize() )
	{
		std::stringstream Error;
		Error << "Stream " << Index << "/" << TotalStreamCount << " doesn't exist";
		throw Soy::AssertException( Error.str() );
	}
	
	return Streams[Index];
}


TStreamMeta TMediaExtractor::GetVideoStream(size_t Index)
{
	Array<TStreamMeta> Streams;
	GetStreams( GetArrayBridge(Streams) );
	auto TotalStreamCount = Streams.GetSize();
	
	//	remove non-video streams
	for ( auto i=size_cast<ssize_t>(Streams.GetSize())-1;	i>=0;	i-- )
	{
		if ( !SoyMediaFormat::IsVideo( Streams[i].mCodec ) )
			Streams.RemoveBlock( i, 1 );
	}
	
	if ( Index >= Streams.GetSize() )
	{
		std::stringstream Error;
		Error << "Video stream " << Index << "/" << TotalStreamCount << " doesn't exist";
		throw Soy::AssertException( Error.str() );
	}
	
	return Streams[Index];
}


void TMediaEncoder::PushFrame(std::shared_ptr<TMediaPacket>& Packet,std::function<bool(void)> Block)
{
	mOutput->PushPacket( Packet, Block );
}



TMediaMuxer::TMediaMuxer(std::shared_ptr<TStreamWriter>& Output,std::shared_ptr<TMediaPacketBuffer>& Input,const std::string& ThreadName) :
	SoyWorkerThread	( ThreadName, SoyWorkerWaitMode::Wake ),
	mOutput			( Output ),
	mInput			( Input )
{
	Soy::Assert( mOutput!=nullptr, "TMpeg2TsMuxer output missing");
	Soy::Assert( mInput!=nullptr, "TMpeg2TsMuxer input missing");
	mOnPacketListener = WakeOnEvent( mInput->mOnNewPacket );
	
	Start();
}

TMediaMuxer::~TMediaMuxer()
{
	WaitToFinish();
}



bool TMediaMuxer::CanSleep()
{
	if ( !mInput )
		return true;
	if ( !mInput->HasPackets() )
		return true;
	
	return false;
}


bool TMediaMuxer::Iteration()
{
	//	pop next packet
	if ( !mInput )
		return true;
	
	auto Packet = mInput->PopPacket();
	if ( !Packet )
		return true;
	
	try
	{
		ProcessPacket( Packet, *mOutput );
	}
	catch (std::exception& e)
	{
		std::Debug << __func__ << " error; " << e.what() << std::endl;
	}
	
	return true;
}
