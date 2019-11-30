#ifndef VERSIONCONTROL_H
#define VERSIONCONTROL_H

#include <QLinkedList>
#include <QImage>

class VersionControl
{
    friend class MainWindow;
private:
    struct SideNode {
        /**
         * @brief Construct a new Side Node object.
         * 
         * @param image Image to be contained in the node.
         * @param changes Commit message.
         */
        SideNode(QImage image, QString changes) : currentImage(image), changes(changes) {}
        QImage currentImage;    //!< Image contained in the node.
        QString changes;        //!< Commit message.
    };

    class MasterNode {
        friend class MainWindow;
    private:
        QLinkedList<SideNode>           sideBranch;         // Side branch linked list.
        QImage                          nodeImage;          // Image contained in the  node.
        QString                         changes;            // Commit message.
        int                             sideBranchLength;   // Side branch length.

    public:
        MasterNode(QImage, QString);    //!< MasterNode constructor
        MasterNode(SideNode);           //!< Overloaded MasterNode constructor

    public:
        int                             getBranchLength() const { return sideBranchLength; }
        QString                         getName() const { return changes; }
        void                            commitChanges(QImage, QString);
        void                            reverseCommit();
        const QImage&                   getImageAtIndex(int index);
        bool                            canReverseCommit() const { return sideBranchLength >= 2; }
    };

public:
    VersionControl();
    VersionControl(QImage, QString);

    int                                 getBranchLength() const { return masterBranchLength; }
    void                                commitChanges(QImage, QString);
    void                                reverseCommit();
    const QImage&                       getImageAtIndex(int index);
    QLinkedList<MasterNode>::iterator   getMasterNodeIteratorAtIndex(int index);
    bool                                canReverseCommit() const { return masterBranchLength >= 2; }

private:
    QLinkedList<MasterNode>             masterBranch;                   // Master branch linked list.
    int                                 masterBranchLength = 0;         // Length of master branch.
    static const int                    maxMasterBranchLength = 5;      // Maximum length of master branch.
    static const int                    maxSideBranchLength = 5;        // Maximum length of a side branch.

};

#endif // VERSIONCONTROL_H
