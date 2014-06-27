//  FastHypervolume.cpp
//
//  Author:
//       Esteban López-Camacho <esteban@lcc.uma.es>
//
//  Copyright (c) 2014 Antonio J. Nebro
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <FastHypervolume.h>


/**
 * Constructor
 * Creates a new instance of FastHypervolume
 */
FastHypervolume::FastHypervolume() {
  referencePoint_ = NULL;
  numberOfObjectives_ = 0;
} // FastHypervolume

/**
 * Constructor
 * Creates a new instance of FastHypervolume
 */
FastHypervolume::FastHypervolume(double offset) {
  referencePoint_ = NULL;
  numberOfObjectives_ = 0;
  offset_ = offset;
} // FastHypervolume

/**
 * Destructor
 */
FastHypervolume::~FastHypervolume() {
} // ~FastHypervolume

double FastHypervolume::computeHypervolume(SolutionSet* solutionSet) {
  double hv;
  if (solutionSet->size() == 0) {
    hv = 0.0;
  } else {
    numberOfObjectives_ = solutionSet->get(0)->getNumberOfObjectives();
    referencePoint_ = new Solution(numberOfObjectives_);
    updateReferencePoint(solutionSet);
    if (numberOfObjectives_ == 2) {
      Comparator * comparator =
          new ObjectiveComparator(numberOfObjectives_ - 1, true);
      solutionSet->sort(comparator);
      delete comparator;
      hv = get2DHV(solutionSet);
    } else {
      /*
      updateReferencePoint(solutionSet);
      Front front = new Front(solutionSet.size(), numberOfObjectives_, solutionSet);
      hv = new WFGHV(numberOfObjectives_, solutionSet.size(), referencePoint_).getHV(front);
      */
      cerr << "Fast Hypervolume only works with two objectives for now." << endl;
    }
    delete referencePoint_;
  }

  return hv;
} // computeHypervolume

/**
 * Updates the reference point
 */
void FastHypervolume::updateReferencePoint(SolutionSet * solutionSet) {
  double * maxObjectives = new double[numberOfObjectives_];
  for (int i = 0; i < numberOfObjectives_; i++) {
    maxObjectives[i] = 0;
  }

  for (int i = 0; i < solutionSet->size(); i++) {
    for (int j = 0; j < numberOfObjectives_; j++) {
      if (maxObjectives[j] < solutionSet->get(i)->getObjective(j)) {
        maxObjectives[j] = solutionSet->get(i)->getObjective(j);
      }
    }
  }

  for (int i = 0; i < referencePoint_->getNumberOfObjectives(); i++) {
    referencePoint_->setObjective(i, maxObjectives[i] + offset_);
  }

  delete [] maxObjectives;
} // updateReferencePoint

/**
 * Computes the HV of a solutiontype set.
 * REQUIRES: The problem is bi-objective
 * REQUIRES: The archive is ordered in descending order by the second objective
 *
 * @return
 */
double FastHypervolume::get2DHV(SolutionSet * solutionSet) {
  double hv = 0.0;
  if (solutionSet->size() > 0) {
    hv = fabs((solutionSet->get(0)->getObjective(0) - referencePoint_->getObjective(0)) *
      (solutionSet->get(0)->getObjective(1) - referencePoint_->getObjective(1)));

    for (int i = 1; i < solutionSet->size(); i++) {
      double tmp =
        fabs((solutionSet->get(i)->getObjective(0) - referencePoint_->getObjective(0)) *
          (solutionSet->get(i)->getObjective(1) - solutionSet->get(i-1)->getObjective(1)));
      hv += tmp;
    }
  }
  return hv;
} // get2DHV

void FastHypervolume::computeHVContributions(SolutionSet * solutionSet) {
  double * contributions = new double[solutionSet->size()];
  double solutionSetHV = 0;

  solutionSetHV = computeHypervolume(solutionSet);

  for (int i = 0; i < solutionSet->size(); i++) {
    Solution * currentPoint = solutionSet->get(i);
    solutionSet->remove(i);

    if (numberOfObjectives_ == 2) {
      //updateReferencePoint(solutionSet);
      //solutionSet.sort(new ObjectiveComparator(numberOfObjectives_-1, true));
      contributions[i] = solutionSetHV - get2DHV(solutionSet);
    } else {
      /*
      Front front = new Front(solutionSet.size(), numberOfObjectives_, solutionSet);
      double hv =
        new WFGHV(numberOfObjectives_, solutionSet.size(), referencePoint_).getHV(front);
      contributions[i] = solutionSetHV - hv;
      */
      cerr << "Fast Hypervolume only works with two objectives for now." << endl;
    }
    solutionSet->add(i, currentPoint);
  }

  for (int i = 0; i < solutionSet->size(); i++) {
    solutionSet->get(i)->setCrowdingDistance(contributions[i]);
  }
} // computeHVContributions
