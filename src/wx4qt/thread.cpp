/////////////////////////////////////////////////////////////////////////////
/**
 * @file wx/thread.cpp
 * @brief Implementation file for wxCriticalSection / wxCriticalSectionLocker class.
 *
 * Use STL to make same behaviors than wxWidgets.
 *
 * @author Stéphane Château
 * @date Created: 2025/09/02
 * @date Modified:
 * @copyright Copyright © Stéphane Château
 * @license wxWindows License
 */
/////////////////////////////////////////////////////////////////////////////
#include <wx/thread.h>

void wxCriticalSection::Enter()
{
    m_mutex.lock();
}

bool wxCriticalSection::TryEnter()
{
    return m_mutex.try_lock();
}

void wxCriticalSection::Leave()
{
    m_mutex.unlock();
}

wxCriticalSectionLocker::wxCriticalSectionLocker(wxCriticalSection& criticalSection)
    : m_criticalSection(criticalSection)
{
    m_criticalSection.Enter();
}

wxCriticalSectionLocker::~wxCriticalSectionLocker()
{
    m_criticalSection.Leave();
}
