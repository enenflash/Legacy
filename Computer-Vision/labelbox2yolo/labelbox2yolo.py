import json
import os

# Map of label names to YOLO class ids
label_map = {
    "blue_goal": 0,  # Example: map "blue_goal" to class_id 0 in YOLO
    "yellow_goal": 1,
    # Add other labels here if needed
}

# Directory to store YOLO annotations for each video
output_dir = 'yolo_annotations'
os.makedirs(output_dir, exist_ok=True)

with open("Export v2 project - Goal Detection - 11_10_2024.ndjson", "r") as f:
    data = [json.loads(line) for line in f]

# Process each annotation
for annotation in data:  # Assuming 'data' is the list of annotations from your NDJSON
    # Get the video file name (or ID) from 'media_attributes' or a specific field in the annotation
    video_id = annotation['data_row']['external_id']  # Example: "video_1.mp4"
    
    # Create a directory for this video (if it doesn't exist)
    video_output_dir = os.path.join(output_dir, video_id)
    os.makedirs(video_output_dir, exist_ok=True)
    
    # Loop through the frames and their annotations
    for frame_number, frame_data in annotation['projects']['cm39qihof07kr07uo9u0a4si4']['labels'][0]['annotations']['frames'].items():
        # Create a YOLO annotation file for this frame
        annotation_filename = os.path.join(video_output_dir, f"frame_{frame_number}.txt")
        
        with open(annotation_filename, 'w') as annot_file:
            # Iterate over the objects in the current frame
            for obj_id, obj in frame_data['objects'].items():
                # Extract label and bounding box
                label = obj["value"]  # This will be the "blue_goal"
                class_id = label_map[label]  # Map the label to YOLO class_id
                
                # Get bounding box values
                bbox = obj["bounding_box"]
                x_min = bbox["left"]
                y_min = bbox["top"]
                box_width = bbox["width"]
                box_height = bbox["height"]
                
                # Get image dimensions
                image_width = annotation['media_attributes']['width']
                image_height = annotation['media_attributes']['height']
                
                # Convert bounding box to YOLO format (normalized)
                x_center = (x_min + box_width / 2) / image_width
                y_center = (y_min + box_height / 2) / image_height
                width = box_width / image_width
                height = box_height / image_height
                
                # Store YOLO annotation in the format: class_id x_center y_center width height
                annot_file.write(f"{class_id} {x_center} {y_center} {width} {height}\n")

print("YOLO annotations generated successfully.")
