#include "WordDescriptionConcurrentUpdater.h"

#include <iostream>
#include <thread>

namespace glossary
{

WordDescriptionConcurrentUpdater::WordDescriptionConcurrentUpdater(
    std::shared_ptr<wordDescriptionDownloader::WordDescriptionDownloader> wordDescriptionDownloaderInit,
    std::shared_ptr<wordDescriptionRepository::WordDescriptionRepository> wordDescriptionRepositoryInit)
    : wordDescriptionDownloader{std::move(wordDescriptionDownloaderInit)},
      wordDescriptionRepository{std::move(wordDescriptionRepositoryInit)}
{
}

void WordDescriptionConcurrentUpdater::update(const dictionaryService::EnglishWords& englishWords)
{
    const auto amountOfThreads = getAmountOfThreads();
    std::vector<std::thread> threadPool;
    threadPool.reserve(amountOfThreads);

    const auto englishWordsWithoutWordDescriptions = getEnglishWordsWithoutWordDescription(englishWords);
    utils::ThreadSafeQueue<wordDescriptionRepository::EnglishWord> englishWordsQueue{
        englishWordsWithoutWordDescriptions};
    utils::ThreadSafeQueue<wordDescriptionRepository::WordDescription> wordsDescriptions;

    for (unsigned threadNumber = 0; threadNumber < amountOfThreads; threadNumber++)
    {
        threadPool.emplace_back(std::thread(&WordDescriptionConcurrentUpdater::loadingWordDescriptionWorker,
                                            this, std::ref(englishWordsQueue), std::ref(wordsDescriptions)));
    }

    for (auto& thread : threadPool)
    {
        thread.join();
    }

    loadWordsDescriptionsIntoRepository(wordsDescriptions.popAll());
}

unsigned WordDescriptionConcurrentUpdater::getAmountOfThreads() const
{
    return supportedThreadsCalculator.calculate();
}

wordDescriptionRepository::EnglishWords
WordDescriptionConcurrentUpdater::getEnglishWordsWithoutWordDescription(
    const wordDescriptionRepository::EnglishWords& englishWords) const
{
    wordDescriptionRepository::EnglishWords englishWordsWithoutWordDescription;

    for (const auto& englishWord : englishWords)
    {
        if (not wordDescriptionRepository->contains(englishWord))
        {
            englishWordsWithoutWordDescription.emplace_back(englishWord);
        }
    }
    return englishWordsWithoutWordDescription;
}

void WordDescriptionConcurrentUpdater::loadingWordDescriptionWorker(
    utils::ThreadSafeQueue<wordDescriptionRepository::EnglishWord>& englishWords,
    utils::ThreadSafeQueue<wordDescriptionRepository::WordDescription>& wordsDescriptions)
{
    while (const auto currentEnglishWord = englishWords.pop())
    {
        if (const auto downloadedWordDescription = downloadWordDescription(*currentEnglishWord))
        {
            wordsDescriptions.push(*downloadedWordDescription);
        }
    }
}

boost::optional<wordDescriptionRepository::WordDescription>
WordDescriptionConcurrentUpdater::downloadWordDescription(
    const wordDescriptionRepository::EnglishWord& englishWord)
{
    try
    {
        return wordDescriptionDownloader->tryDownloadWordDescription(englishWord);
    }
    catch (const std::exception&)
    {
        return boost::none;
    }
}

void WordDescriptionConcurrentUpdater::loadWordsDescriptionsIntoRepository(
    const wordDescriptionRepository::WordsDescriptions& wordsDescriptions)
{
    for (const auto& wordDescription : wordsDescriptions)
    {
        wordDescriptionRepository->addWordDescription(wordDescription);
    }
}

}