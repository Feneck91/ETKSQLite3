/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/thread.h
 * @brief Header file for wxCriticalSection / wxCriticalSectionLocker class.
 *
 * Use STL to make same behaviors than wxWidgets.
 *
 * @author Stéphane Château
 * @date Created: 2014/06/16
 * @date Modified: 2025/09/02
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#ifndef WX_CRITICALSECTION_HEADER
#define WX_CRITICALSECTION_HEADER

#include <mutex>

class wxCriticalSection
{
public:
    // Default constructor
    wxCriticalSection() = default;

    // Enter into critical section
    void Enter();

    // Try enter to critical section (retourne true si réussi)
    bool TryEnter();

    // Quit critical section
    void Leave();

private:
    std::mutex m_mutex; // Standart mutex to manage critical section
};

class wxCriticalSectionLocker
{
public:
    // Constructor that lock critical section
    explicit wxCriticalSectionLocker(wxCriticalSection& criticalSection);

    // Destructor unlock automatically the critical section
    ~wxCriticalSectionLocker();

private:
    wxCriticalSection& m_criticalSection; // Ref to critical section
};

#endif // WX_CRITICALSECTION_HEADER
