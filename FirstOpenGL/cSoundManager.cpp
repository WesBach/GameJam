#include "cSoundManager.h"
#include "Utilities.h"
#include <iostream>
#include <algorithm>
#include <vector>

//Name:		checkForFMODError
//Purpose:	Checks to see the outcome of the FMOD_RESULT and prints an error then exits
//Return:	void
void cSoundManager::checkForFMODError(FMOD_RESULT result) {
	if (result != FMOD_OK)
	{
		fprintf(stderr, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		//exit(-1);
	}
}

void  cSoundManager::playEnemyDamagedSound() {
	mSystem->playSound(mSounds["assets/Songs/enemy-impact.wav"].theSound, 0, true, &mSounds["assets/Songs/enemy-impact.wav"].theChannel);
	this->mSounds["assets/Songs/enemy-impact.wav"].theChannel->setPaused(false);
}

void  cSoundManager::playPlayerDamagedSound() {
	mSystem->playSound(mSounds["assets/Songs/player-impact.wav"].theSound, 0, true, &mSounds["assets/Songs/player-impact.wav"].theChannel);
	this->mSounds["assets/Songs/player-impact.wav"].theChannel->setPaused(false);
}

void cSoundManager::playPlayerShootSound() {
	mSystem->playSound(mSounds["assets/Songs/player-shoot.wav"].theSound, 0, true, &mSounds["assets/Songs/player-shoot.wav"].theChannel);
	this->mSounds["assets/Songs/player-shoot.wav"].theChannel->setPaused(false);
}

void cSoundManager::playEnemyShootSound() {
	mSystem->playSound(mSounds["assets/Songs/enemy-shoot.wav"].theSound, 0, true, &mSounds["assets/Songs/enemy-shoot.wav"].theChannel);
	this->mSounds["assets/Songs/enemy-shoot.wav"].theChannel->setPaused(false);
}

void cSoundManager::playExplosionSound() {
	mSystem->playSound(mSounds["assets/Songs/explosion.wav"].theSound, 0, true, &mSounds["assets/Songs/explosion.wav"].theChannel);
	this->mSounds["assets/Songs/explosion.wav"].theChannel->setPaused(false);
}

//void  cSoundManager::changeBackGroundMusic(std::string levelName) {
//
//	//err = soundSystem->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
//	//err = channel->setVolume(someVolumeBetween0and1);
//	//err = channle->setPaused(false);
//	this->mSounds[levelName].theChannel->setPaused(true);
//}

void  cSoundManager::initSoundManager() {
	// Create the main system object.
	mResult = FMOD::System_Create(&mSystem);
	checkForFMODError(mResult);

	//Initializes the system object, and the msound device.
	mResult = mSystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	checkForFMODError(mResult);
}

void cSoundManager::playHealthSound() {
	mSystem->playSound(mSounds["assets/Songs/health.wav"].theSound, 0, true, &mSounds["assets/Songs/health.wav"].theChannel);
	this->mSounds["assets/Songs/health.wav"].theChannel->setPaused(false);
}

void cSoundManager::playRangeSound() {
	mSystem->playSound(mSounds["assets/Songs/range.wav"].theSound, 0, true, &mSounds["assets/Songs/range.wav"].theChannel);
	this->mSounds["assets/Songs/range.wav"].theChannel->setPaused(false);
}

void  cSoundManager::changeBackGroundMusic(std::string levelName) {
	this->mBackgroundMusicSound->theChannel->setPaused(true);
	//set the new background sound
	this->mBackgroundMusicSound = &mSounds[levelName];
	this->mBackgroundMusicSound->theChannel->setPaused(false);
	this->mBackgroundMusicSound->theChannel->setVolume(float(0.8f));
}

void cSoundManager::setInitialBackgroundSound() {
	this->mBackgroundMusicSound = &mSounds["assets/Songs/Tapestry.wav"];
	this->mBackgroundMusicSound->theChannel->setPaused(false);
	this->mBackgroundMusicSound->theChannel->setVolume(float(0.8f));
}

void cSoundManager::readSoundsFromSoundFile(std::string filename) {
	//TODO:: read sounds from sound file into sound map
	std::ifstream configFile(filename.c_str());
	std::string tempString = "";
	int numberOfFiles = 0;
	std::vector<std::string> soundTitles;

	if (!configFile.is_open())
	{	// Didn't open file, so return
		std::cout << "Error Opening Sound Info File!" << std::endl;
	}
	// File is open, let's read it

	//get the number of files to read 
	ReadFileToToken(configFile, "NumberOfLoopSounds");
	configFile >> numberOfFiles;
	ReadFileToToken(configFile, "START_LOOP_SOUNDS");
	//read that many files in
	for (int i = 0; i < numberOfFiles; i++) {
		tempString = "";
		configFile >> tempString;
		//tell the program where to find the file
		soundTitles.push_back("assets/Songs/" + tempString);
	}

	//if the sound titles vector is greater than 0
	if (soundTitles.size() > 0)
	{
		FMOD::Sound *tempSound;
		FMOD::Channel *tempChannel;
		SoundHelper* tempSoundHelper;
		int tempIndex = 0;

		for (unsigned int i = 0; i < soundTitles.size(); i++)
		{
			tempSoundHelper = new SoundHelper();

			mResult = mSystem->createStream(soundTitles[i].c_str(), FMOD_LOOP_NORMAL | FMOD_DEFAULT, 0, &tempSound);
			checkForFMODError(mResult);

			//pause all the sounds
			mResult = mSystem->playSound(tempSound, 0, true, &tempChannel);

			//check for errors
			checkForFMODError(mResult);
			//set up the temp sound helper 
			tempSoundHelper->groupIndex = tempIndex;
			tempSoundHelper->theChannel = tempChannel;
			tempSoundHelper->theSound = tempSound;
			//insert the temp sound helper into the map
			this->mSounds[soundTitles[i]] = *tempSoundHelper;
		}
	}

	//clear the sound titles
	soundTitles.clear();

	ReadFileToToken(configFile, "NumberOfNormalSounds");
	configFile >> numberOfFiles;
	ReadFileToToken(configFile, "START_NORMAL_SOUNDS");
	//read that many files in
	for (int i = 0; i < numberOfFiles; i++) {
		tempString = "";
		configFile >> tempString;
		//tell the program where to find the file
		soundTitles.push_back("assets/Songs/" + tempString);
	}

	if (soundTitles.size() > 0)
	{
		FMOD::Sound *tempSound;
		FMOD::Channel *tempChannel;
		SoundHelper* tempSoundHelper;
		int tempIndex = 0;

		for (unsigned int i = 0; i < soundTitles.size(); i++)
		{
			tempSoundHelper = new SoundHelper();

			mResult = mSystem->createStream(soundTitles[i].c_str(), FMOD_DEFAULT, 0, &tempSound);
			checkForFMODError(mResult);

			//pause all the sounds
			mResult = mSystem->playSound(tempSound, 0, true, &tempChannel);

			//check for errors
			checkForFMODError(mResult);
			//set up the temp sound helper 
			tempSoundHelper->groupIndex = tempIndex;
			tempSoundHelper->theChannel = tempChannel;
			tempSoundHelper->theSound = tempSound;
			//insert the temp sound helper into the map
			this->mSounds[soundTitles[i]] = *tempSoundHelper;
		}
	}

}
