#include "VersionControl.h"

VersionControl::VersionControl(QImage image)
{
    masterBranch.push_front(MasterNode(image));
    masterBranchLength = 1;
}

// Master Node constructor
VersionControl::MasterNode::MasterNode(QImage image)
{
    sideBranch.push_front(image);
    sideBranchLength = 1;
}

void VersionControl::MasterNode::commitImage(QImage image)
{
    if (!canCommitImage())
        return;
    sideBranch.push_front(image);
    ++sideBranchLength;
}

void VersionControl::MasterNode::reverseCommit()
{
    if (!canReverseCommit())
        return;
    sideBranch.pop_front();
    --sideBranchLength;
}

const QImage& VersionControl::MasterNode::getImageAtIndex(int index)
{
    QLinkedList<QImage>::iterator it = sideBranch.begin();
    for (int i = 0; i < index - 1; ++i) {
        ++it;
    }
    return *it;
}

void VersionControl::commitImage(QImage image)
{
    if (masterBranchLength + 1 <= maxMasterBranchLength) {
        masterBranch.push_front(MasterNode(image));
        ++masterBranchLength;
    }
    else {
        masterBranch.pop_back();
        masterBranch.push_front(MasterNode(image));
    }
}

void VersionControl::reverseCommit()
{
    if (!canReverseCommit())
        return;
    masterBranch.pop_front();
    --masterBranchLength;
}

const QImage& VersionControl::getImageAtIndex(int index)
{
    QLinkedList<MasterNode>::iterator it = masterBranch.begin();
    for (int i = 0; i < index - 1; ++i) {
        ++it;
    }
    return it->getImageAtIndex(0);
}
