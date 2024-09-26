#include "EigenFaceSearcher.h"

#include <QtCore/QList>

#include "EigenFaceSearchResultList.h"
#include "EigenFaceSearchPerson.h"
#include "EigenFaceSearchSettings.h"
#include "EigenFaceTemplate.h"
#include "EigenFaceVector.h"
#include "Return.h"

EigenFaceSearcher::EigenFaceSearcher(void)
    : bestConfidence_i(0)
    , leastDistance_r(999.9)
{
}

void EigenFaceSearcher::clear(void)
{
    faceKey_vector_mhash.clear();
}

bool EigenFaceSearcher::contains(const int faceKey) const
{
    return faceKey_vector_mhash.contains(faceKey);
}

int EigenFaceSearcher::size(void) const
{
    return faceKey_vector_mhash.size();
}

QString EigenFaceSearcher::deleteFace(const int faceKey)
{
    if ( ! faceKey_vector_mhash.contains(faceKey))
        return QString("%1 faceKey not enrolled").arg(faceKey);
    faceKey_vector_mhash.remove(faceKey);
    return QString();
}

void EigenFaceSearcher::enroll(const EigenFaceTemplate & tpl,
                                      const int faceKey,
                                      const int personKey)
{
    foreach(EigenFaceVector vector, tpl.vectorMap().values())
    {
        vector.setKeys(faceKey, personKey);
        faceKey_vector_mhash.insert(faceKey, vector);
    }
}

int EigenFaceSearcher::bestConfidence(void) const
{
    return bestConfidence_i;
}

qreal EigenFaceSearcher::leastDistance(void) const
{
    return leastDistance_r;
}

/*! search()
  *
  * This function does its work by passing an EigenFaceSearchResultList
  * through several filters.
  *
  * distanceList is the computed distances for every search vector versus
  *     every (selected) enrolled vector. It contains one result per "person".
  *     It is limited by MaxDistance.
  * rankedList is sorted by distance and still contains one result per
  *     "person" but that single result has a rank assigned.
  * personList has each person's individual results combined into one entry.
  * combinedList has the distances of the individual results combined
  *     together for that person's distance and confidence score.
  *     It is limited by MinConfidence.
  * sortedList has been sorted by least distance and tiers assigned. It has been
  *     limited by maxResults.
  * tieredList has had search result tiers assigned to the sorted results.
  */
EigenFaceSearchResultList EigenFaceSearcher::
    search(const EigenFaceSearchSettings::Mode mode,
           const EigenFaceTemplate & tpl,
           EigenFaceSearchSettings * settings,
           const QList<int> & faceKeyList)
{
    int personMethod = settings->personMethod(mode, faceKey_vector_mhash.size());
    bool personMode = settings->getPersonMode();
    qreal maxDistance = settings->fMaxDistance();
    qreal minDistance = settings->fMinDistance();
    int maxFaces = settings->maxFaces(faceKey_vector_mhash.size());
//    int minConfidence = settings->getMinConfidence();

    EigenFaceSearchResultList distanceList = computeDistance(faceKeyList, tpl,
                                                             maxDistance, minDistance);
    EigenFaceSearchResultList rankedList = rankResults(distanceList, maxFaces);
    EigenFaceSearchResultList personList = personMode
                                            ? collectPersons(rankedList)
                                            : rankedList;

    EigenFaceSearchResultList combinedList;
    if (personMode)
        switch (personMethod)
        {
        case 0:
        default:
        case 6: combinedList = combineBySearchKey(personList, settings);break;
        case 5: combinedList = combineByVector(personList, settings);   break;
        case 4: combinedList = combineAll(personList, settings);        break;
        case 3: combinedList = combineByRank(personList, settings);     break;
        case 2: combinedList = combineByEnroll(personList, settings);   break;
        case 1: combinedList = combineBest(personList, settings);       break;
        }
    else
        combinedList = removeDuplicatePersons(personList, settings);

    EigenFaceSearchResultList sortedList = sortResults(combinedList, settings);
    EigenFaceSearchResultList tieredList;
    switch (settings->getTierMethod())
    {
    default:
    case 0:
    case 1:
        tieredList = setFixedTiers(sortedList, settings);
        break;
    }
    tieredList.setBests();
    bestConfidence_i = tieredList.bestConfidence();
    leastDistance_r = tieredList.leastDistance();
    return tieredList;
}

EigenFaceSearchResultList EigenFaceSearcher::
    computeDistance(const QList<int> & faceKeyList,
                    const EigenFaceTemplate & tpl,
                    qreal maxDistance,
                    qreal minDistance)
{
    QList<EigenFaceVector> searchVectors = tpl.vectorMap().values();
    QList<EigenFaceVector> enrollVectors;
    EigenFaceSearchResultList distanceList;

    int n = searchVectors.size();
    for (int x = 0, sk = -1; x < n; ++x, --sk)
        searchVectors[x].setFaceKey(sk);

    if (faceKeyList.isEmpty())
        enrollVectors = faceKey_vector_mhash.values();
    else
        foreach(int faceKey, faceKeyList)
            enrollVectors << faceKey_vector_mhash.values(faceKey);

    foreach(EigenFaceVector enrollVector, enrollVectors)
        foreach(EigenFaceVector searchVector, searchVectors)
        {
            qreal distance = enrollVector.distance(searchVector);
            if (distance <= maxDistance && distance >= minDistance)
                distanceList
                        << EigenFaceSearchPerson(enrollVector.personKey(),
                                                 enrollVector.faceKey(),
                                                 searchVector.faceKey(),
                                                 enrollVector.vectorKey(),
                                                 searchVector.vectorKey(),
                                                 distance,
                                                 EigenFaceSearchSettings::
                                                     confidence(distance));
        }
    return distanceList;
}

EigenFaceSearchResultList EigenFaceSearcher::
    rankResults(const EigenFaceSearchResultList & distanceList,
                const int maxFaces)
{
    QMultiMap<qreal, EigenFaceSearchPerson> distanceMap;
    foreach(EigenFaceSearchPerson efsp, distanceList)
        distanceMap.insert(efsp.getDistance(), efsp);

    EigenFaceSearchResultList rankedList;
    int rank = 0;
    foreach(EigenFaceSearchPerson efsp, distanceMap.values())
    {
        efsp.setFirstRank(++rank);
        rankedList << efsp;
        if (efsp.size() >= maxFaces)
            break;
    }

    return rankedList;
}

EigenFaceSearchResultList EigenFaceSearcher::
    collectPersons(const EigenFaceSearchResultList & rankedList)
{
    QMultiHash<int, EigenFaceSearchPerson> personHash;
    foreach(EigenFaceSearchPerson efsp, rankedList)
    {
        int personKey = efsp.getPersonKey();
        if (personKey && personHash.contains(personKey))
        {
            EigenFaceSearchPerson hashPerson = personHash.value(personKey);
            hashPerson.append(efsp);
            personHash.replace(personKey, hashPerson);
        }
        else
            personHash.insert(personKey, efsp);
    }

    EigenFaceSearchResultList personList;
    foreach(EigenFaceSearchPerson efsp, personHash.values())
        personList << efsp;
    return personList;
}

EigenFaceSearchResultList EigenFaceSearcher::
    removeDuplicatePersons(const EigenFaceSearchResultList & personList,
                       EigenFaceSearchSettings * settings)
{
    (void)settings;
    EigenFaceSearchResultList combinedList;
    QSet<int> personKeysUsed;

    foreach (EigenFaceSearchPerson efsp, personList)
    {
        int personKey = efsp.getPersonKey();
        if ( ! personKeysUsed.contains(personKey))
        {
            combinedList.append(efsp);
            personKeysUsed.insert(personKey);
        }
    }
    return combinedList;
}

EigenFaceSearchResultList EigenFaceSearcher::
    combineBySearchKey(const EigenFaceSearchResultList & personList,
                       EigenFaceSearchSettings * settings)
{
    const int minConfidence = settings->getMinConfidence();
    const int maxPersonFaces = settings->getMaxPersonFaces();
    EigenFaceSearchResultList combinedList;

    foreach (EigenFaceSearchPerson efsp, personList)
    {
        QList<EigenFaceSearchResult> personResults = efsp.results();
        QSet<int> searchKeySet;
        efsp.clearResults();
        qreal distance = 1.0;
        qreal personRank = 0.0;
        foreach(EigenFaceSearchResult efsr, personResults)
        {
            int searchKey = efsr.getSearchKey();
            if ( ! searchKeySet.contains(searchKey))
            {
                searchKeySet.insert(searchKey);
                distance *= qMin(1.0, efsr.getDistance()
                                          * (1.0 + (0.2 * personRank++)));
                efsp.append(efsr);
            }
            if (efsp.size() >= maxPersonFaces)
                break;
        }
        int confidence = EigenFaceSearchSettings::confidence(distance);
        if (confidence >= minConfidence)
        {
            efsp.setDistance(distance);
            efsp.setConfidence(confidence);
            combinedList << efsp;
        }
    }
    return combinedList;
}

EigenFaceSearchResultList EigenFaceSearcher::
    combineBest(const EigenFaceSearchResultList & personList,
                EigenFaceSearchSettings * settings)
{
    int minConfidence = settings->getMinConfidence();
    return computePersonDistance(personList,
                                 minConfidence,
                                 1,
                                 0);
}

EigenFaceSearchResultList EigenFaceSearcher::
    combineByEnroll(const EigenFaceSearchResultList & personList,
                      EigenFaceSearchSettings * settings)
{
    int maxPersonFaces = settings->getMaxPersonFaces();
    int minConfidence = settings->getMinConfidence();
    return computePersonDistance(personList,
                                 minConfidence,
                                 maxPersonFaces,
                                 faceKey_vector_mhash.size());
}

EigenFaceSearchResultList EigenFaceSearcher::
    combineByVector(const EigenFaceSearchResultList & personList,
                      EigenFaceSearchSettings * settings)
{
    int maxPersonFaces = settings->getMaxPersonFaces();
    int minConfidence = settings->getMinConfidence();
    return computePersonDistance(personList,
                                 minConfidence,
                                 maxPersonFaces,
                                 personList.totalVectors()); // scale by position
}

EigenFaceSearchResultList EigenFaceSearcher::
    combineByRank(const EigenFaceSearchResultList & personList,
                  EigenFaceSearchSettings * settings)
{
    int maxPersonFaces = settings->getMaxPersonFaces();
    int minConfidence = settings->getMinConfidence();
    return computePersonDistance(personList,
                                 minConfidence,
                                 maxPersonFaces,
                                 0); // linear scale
}

EigenFaceSearchResultList EigenFaceSearcher::
    combineAll(const EigenFaceSearchResultList & personList,
               EigenFaceSearchSettings * settings)
{
    int maxPersonFaces = settings->getMaxPersonFaces();
    int minConfidence = settings->getMinConfidence();
    return computePersonDistance(personList,
                                 minConfidence,
                                 maxPersonFaces,
                                 -1); // no rank degradation
}

EigenFaceSearchResultList EigenFaceSearcher::
    computePersonDistance(const EigenFaceSearchResultList personList,
                          const int minConfidence,
                          const int maxPersonFaces,
                          const int topRank)
{
    EigenFaceSearchResultList combinedList;
    qreal fTopRank = topRank;

    foreach (EigenFaceSearchPerson efsp, personList)
    {
        int nResults = efsp.size();
        if (maxPersonFaces && nResults > maxPersonFaces)
            nResults = maxPersonFaces;

        QList<EigenFaceSearchResult> efsrs = efsp.results();
        EigenFaceSearchResult bestResult = efsrs.takeFirst();
        qreal distance = bestResult.getDistance();

        QSet<int> usedFaceKeys;
        usedFaceKeys.insert(bestResult.getFaceKey());
        QList<EigenFaceSearchResult> usedResults;
        usedResults << bestResult;
        efsp.setResults(usedResults);
        qreal personRank = 1;

        foreach(EigenFaceSearchResult efsr, efsrs)
        {
            if (efsp.size() >= nResults)
                continue;
            int resultFaceKey = efsr.getFaceKey();

            qreal resultRank = efsr.getRank();
            qreal resultDistance = efsr.getDistance();
            qreal c = 1.0;
            if (topRank < 0)
                ; // leave at one
            else if (topRank)
                c = 1.0 + resultRank / fTopRank;
            else // topRank is zero
                // c = 2.0 - (1.0 / (1.0 + resultRank));
                c = 1.0 + (0.05 * ++personRank);
            distance *= qMin(1.0, c * resultDistance);

            efsp.append(efsr);
            usedFaceKeys.insert(resultFaceKey);
        }

        int confidence = EigenFaceSearchSettings::confidence(distance);
        if (confidence > minConfidence)
        {
            efsp.setBestDistance(bestResult.getDistance());
            efsp.setBestFaceKey(bestResult.getFaceKey());
            efsp.setConfidence(confidence);
            efsp.setDistance(distance);
            combinedList << efsp;
        }
    }

    return combinedList;
}

EigenFaceSearchResultList EigenFaceSearcher::
    sortResults(const EigenFaceSearchResultList & combinedList,
                EigenFaceSearchSettings * settings)
{
    int maxResults = settings->maxResults(faceKey_vector_mhash.size());

    QMultiMap<qreal, EigenFaceSearchPerson> distanceMap;
    foreach(EigenFaceSearchPerson efsp, combinedList)
        distanceMap.insert(efsp.getDistance(), efsp);

    int rank = 0;
    EigenFaceSearchResultList sortedResults;
    foreach(EigenFaceSearchPerson efsp, distanceMap.values())
    {
        efsp.setRank(++rank);
        sortedResults << efsp;
        if (maxResults && rank >= maxResults)
            break;
    }
    return sortedResults;
}

EigenFaceSearchResultList EigenFaceSearcher::
    setFixedTiers(const EigenFaceSearchResultList & sortedList,
                  EigenFaceSearchSettings * settings)
{
    int strongPersonCount = settings->getStrongPersonCount();
    EigenFaceSearchResultList tieredList;

    foreach(EigenFaceSearchPerson efsp, sortedList)
    {
        int confidence = efsp.getConfidence();
        EigenFaceSearchTier tier = settings->tierForConfidence(confidence);
        if (1 == efsp.getRank() && EigenFaceSearchTier::Strong == tier)
        {
            if (1 == sortedList.size())
                tier = EigenFaceSearchTier::Best;
            else
            {
                int secondConfidence = sortedList.at(1).getConfidence();
                EigenFaceSearchTier secondTier
                        = settings->tierForConfidence(secondConfidence);
                if (secondTier < EigenFaceSearchTier::Strong)
                    tier = EigenFaceSearchTier::Best;
            }
        }
        else if (strongPersonCount <= efsp.getRank()
                 && EigenFaceSearchTier::Strong == tier)
        {
            tieredList.setEachTier(EigenFaceSearchTier::Possible);
            tier = EigenFaceSearchTier::Possible;
        }
        efsp.setTier(tier);
        tieredList << efsp;
    }
    return tieredList;
}
