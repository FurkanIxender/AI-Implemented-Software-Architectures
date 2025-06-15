import json
from reportlab.lib import colors
from reportlab.lib.pagesizes import letter
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle

def parse_chat_json(json_file):
    with open(json_file, 'r', encoding='utf-8') as f:
        chat_data = json.load(f)
    
    conversation = []
    for request in chat_data['requests']:
        # Get user message
        user_message = request['message']['text']
        
        # Get AI response
        ai_response = ""
        for response in request['response']:
            if 'value' in response:
                ai_response += response['value']
        
        conversation.append({
            'user': user_message,
            'ai': ai_response
        })
    
    return conversation

def export_to_pdf(conversation, output_file):
    doc = SimpleDocTemplate(output_file, pagesize=letter)
    styles = getSampleStyleSheet()
    
    # Create custom styles
    styles.add(ParagraphStyle(
        name='UserMessage',
        parent=styles['Normal'],
        textColor=colors.blue,
        spaceAfter=12
    ))
    
    styles.add(ParagraphStyle(
        name='AIResponse',
        parent=styles['Normal'],
        textColor=colors.black,
        spaceAfter=20
    ))
    
    # Build PDF content
    content = []
    for entry in conversation:
        # Add user message
        content.append(Paragraph(f"User: {entry['user']}", styles['UserMessage']))
        content.append(Spacer(1, 12))
        
        # Add AI response
        ai_text = entry['ai']
        
        # Split AI response into text and code blocks
        parts = ai_text.split('````')
        for i, part in enumerate(parts):
            if i % 2 == 0:  # Regular text
                if part.strip():
                    content.append(Paragraph(part, styles['AIResponse']))
            else:  # Code block
                lines = part.split('\n')
                if len(lines) > 1:
                    language = lines[0]
                    code = '\n'.join(lines[1:])
                    content.append(Paragraph(f"Code ({language}):", styles['Code']))
                    content.append(Paragraph(code, styles['Code']))
            
            content.append(Spacer(1, 12))
    
    # Build the PDF
    doc.build(content)

if __name__ == "__main__":
    input_file = "chat.json"  # Your chat JSON file
    output_file = "chat_export.pdf"
    
    try:
        conversation = parse_chat_json(input_file)
        export_to_pdf(conversation, output_file)
        print(f"Successfully exported chat to {output_file}")
    except Exception as e:
        print(f"Error: {str(e)}")