/*
 Copyright (C) 2010-2014 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SmartAttributeEditorManager.h"

#include "CollectionUtils.h"
#include "View/MapDocument.h"
#include "View/SmartChoiceEditor.h"
#include "View/SmartChoiceEditorMatcher.h"
#include "View/SmartColorEditor.h"
#include "View/SmartDefaultAttributeEditor.h"
#include "View/SmartAttributeEditor.h"
#include "View/SmartAttributeEditorMatcher.h"
#include "View/SmartSpawnflagsEditor.h"

#include <wx/panel.h>
#include <wx/sizer.h>

namespace TrenchBroom {
    namespace View {
        SmartAttributeEditorManager::SmartAttributeEditorManager(wxWindow* parent, View::MapDocumentWPtr document) :
        wxPanel(parent),
        m_document(document),
        m_name("") {
            createEditors();
            activateEditor(defaultEditor(), "");
            bindObservers();
        }
        
        SmartAttributeEditorManager::~SmartAttributeEditorManager() {
            unbindObservers();
            deactivateEditor();
        }

        void SmartAttributeEditorManager::switchEditor(const Model::AttributeName& name, const Model::AttributableList& attributables) {
            EditorPtr editor = selectEditor(name, attributables);
            activateEditor(editor, name);
            updateEditor();
        }

        void SmartAttributeEditorManager::createEditors() {
            m_editors.push_back(MatcherEditorPair(MatcherPtr(new SmartAttributeEditorKeyMatcher("spawnflags")),
                                                  EditorPtr(new SmartSpawnflagsEditor(m_document))));
            m_editors.push_back(MatcherEditorPair(MatcherPtr(new SmartAttributeEditorKeyMatcher("_color", "_sunlight_color", "_sunlight_color2")),
                                                  EditorPtr(new SmartColorEditor(m_document))));
            m_editors.push_back(MatcherEditorPair(MatcherPtr(new SmartChoiceEditorMatcher()),
                                                  EditorPtr(new SmartChoiceEditor(m_document))));
            m_editors.push_back(MatcherEditorPair(MatcherPtr(new SmartAttributeEditorDefaultMatcher()),
                                                  EditorPtr(new SmartDefaultAttributeEditor(m_document))));
        }
        
        void SmartAttributeEditorManager::bindObservers() {
            MapDocumentSPtr document = lock(m_document);
            document->selectionDidChangeNotifier.addObserver(this, &SmartAttributeEditorManager::selectionDidChange);
            document->nodesDidChangeNotifier.addObserver(this, &SmartAttributeEditorManager::nodesDidChange);
        }
        
        void SmartAttributeEditorManager::unbindObservers() {
            if (!expired(m_document)) {
                MapDocumentSPtr document = lock(m_document);
                document->selectionDidChangeNotifier.removeObserver(this, &SmartAttributeEditorManager::selectionDidChange);
                document->nodesDidChangeNotifier.removeObserver(this, &SmartAttributeEditorManager::nodesDidChange);
            }
        }

        void SmartAttributeEditorManager::selectionDidChange(const Selection& selection) {
            MapDocumentSPtr document = lock(m_document);
            switchEditor(m_name, document->allSelectedAttributables());
        }
        
        void SmartAttributeEditorManager::nodesDidChange(const Model::NodeList& nodes) {
            MapDocumentSPtr document = lock(m_document);
            switchEditor(m_name, document->allSelectedAttributables());
        }
        
        SmartAttributeEditorManager::EditorPtr SmartAttributeEditorManager::selectEditor(const Model::AttributeName& name, const Model::AttributableList& attributables) const {
            EditorList::const_iterator it, end;
            for (it = m_editors.begin(), end = m_editors.end(); it != end; ++it) {
                const MatcherEditorPair& pair = *it;
                const MatcherPtr matcher = pair.first;
                if (matcher->matches(name, attributables))
                    return pair.second;
            }
            
            // should never happen
            assert(false);
            return defaultEditor();
        }
    

        SmartAttributeEditorManager::EditorPtr SmartAttributeEditorManager::defaultEditor() const {
            return m_editors.back().second;
        }
        
        void SmartAttributeEditorManager::activateEditor(EditorPtr editor, const Model::AttributeName& name) {
            if (m_activeEditor != editor || !m_activeEditor->usesName(name)) {
                deactivateEditor();
                m_activeEditor = editor;
                m_name = name;
                wxWindow* window = m_activeEditor->activate(this, m_name);
                
                wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
                sizer->Add(window, 1, wxEXPAND);
                SetSizer(sizer);
                Layout();
            }
        }
        
        void SmartAttributeEditorManager::deactivateEditor() {
            if (m_activeEditor != NULL) {
                m_activeEditor->deactivate();
                m_activeEditor = EditorPtr();
                m_name = "";
            }
        }

        void SmartAttributeEditorManager::updateEditor() {
            if (m_activeEditor != NULL) {
                MapDocumentSPtr document = lock(m_document);
                m_activeEditor->update(document->allSelectedAttributables());
            }
        }
    }
}
