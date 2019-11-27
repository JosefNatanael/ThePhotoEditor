#ifndef VERSIONCONTROL_H
#define VERSIONCONTROL_H

#include <QLinkedList>
#include <QImage>

class VersionControl
{
    friend class MainWindow;
private:
    struct SideNode {
        SideNode(QImage image, QString changes) : currentImage(image), changes(changes) {}
        QImage currentImage;
        QString changes;
    };

    class MasterNode {
        friend class MainWindow;

    private:
        QLinkedList<SideNode>       sideBranch;
        QImage                      nodeImage;
        QString                     changes;
        int                         sideBranchLength;

    public:
        MasterNode(QImage, QString);
        MasterNode(SideNode);

    public:
        int                         getBranchLength() const { return sideBranchLength; }
        QString                     getName() const { return changes; }
        void                        commitChanges(QImage, QString);
        void                        reverseCommit();
        const QImage&               getImageAtIndex(int index);
        bool                        canCommitImage() const { return sideBranchLength <= maxSideBranchLength - 1; }
        bool                        canReverseCommit() const { return sideBranchLength >= 2; }

    };

public:
    VersionControl();
    VersionControl(QImage, QString);

    int                         getBranchLength() const { return masterBranchLength; }
    void                        commitChanges(QImage, QString);
    void                        reverseCommit();
    const QImage&               getImageAtIndex(int index);
    bool                        canReverseCommit() const { return masterBranchLength >= 2; }

private:
    QLinkedList<MasterNode>     masterBranch;
    int                         masterBranchLength = 0;
    static const int            maxMasterBranchLength = 5;
    static const int            maxSideBranchLength = 5;

};

#endif // VERSIONCONTROL_H
