package ua.kh.bars.web;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.multipart.MultipartFile;
import ua.kh.bars.services.EventService;
import ua.kh.bars.services.UserService;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Created by green on 3/20/16.
 */

@Controller
@RequestMapping("/upload")
public class BatchUploadController {

    @Autowired
    UserService userService;

    @Autowired
    EventService eventService;

    @RequestMapping(method = RequestMethod.GET)
    public String showUploadForm() {
        return "batch/uploadForm";
    }

    @RequestMapping(method = RequestMethod.POST)
    public String handleBatchUpload(
            @RequestParam("file") MultipartFile[] files,
            Model model) throws IOException {

        int uploadedFiles = files.length;
        int uploadedEvents = 0;
        int uploadedUsers = 0;
        for(MultipartFile file : files) {
            String content = getFileContent(file);
            JSONObject obj = new JSONObject(content);
            uploadedEvents += saveEvents(obj);
            uploadedUsers += saveUsers(obj);
        }
        model.addAttribute("batchFiles", uploadedFiles);
        model.addAttribute("uploadedEvents", uploadedEvents);
        model.addAttribute("uploadedUsers", uploadedUsers);
        return "batch/upload";
    }

    private int saveUsers(JSONObject obj) {
        int uploadedUsers = 0;
        JSONArray users = obj.getJSONArray("users");
        for(int i=0; i<users.length(); i++) {
            String name = users.getJSONObject(i).getString("name");
            String email = users.getJSONObject(i).getString("email");
            String username = users.getJSONObject(i).getString("username");
            Boolean isEnabled = users.getJSONObject(i).getBoolean("enabled");
            userService.create(name, email, username, isEnabled);
            uploadedUsers++;
        }
        return uploadedUsers;
    }

    private int saveEvents(JSONObject obj) {
        int uploadedEvents = 0;
        JSONArray events = obj.getJSONArray("events");
        for(int i=0; i<events.length(); i++) {
            String name = events.getJSONObject(i).getString("name");
            Long auditoriumId = events.getJSONObject(i).getLong("auditorium_id");
            int price = events.getJSONObject(i).getInt("price");
            eventService.create(name, auditoriumId, price);
            uploadedEvents++;
        }
        return uploadedEvents;
    }

    private String getFileContent(MultipartFile file) throws IOException {
        InputStream stream = file.getInputStream();
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(stream));
        String content = "";
        String line;
        while ((line = bufferedReader.readLine()) != null) {
            content += line;
        }
        return content;
    }

}
