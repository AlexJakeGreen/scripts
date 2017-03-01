package ua.kh.bars.web;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.propertyeditors.CustomDateEditor;
import org.springframework.security.authentication.encoding.ShaPasswordEncoder;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.WebDataBinder;
import org.springframework.web.bind.annotation.*;
import ua.kh.bars.entities.User;
import ua.kh.bars.services.UserService;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by green on 3/19/16.
 */

@Controller@RequestMapping("/users")
public class UserController {

    @Autowired
    UserService userService;

    @Autowired
    ShaPasswordEncoder shaPasswordEncoder;

    @RequestMapping(method = RequestMethod.GET)
    public String indexUsers(Model model) {
        Iterable<User> users = userService.findAll();
        model.addAttribute("users", users);
        return "users/index";
    }

    @RequestMapping(value = "/new", method = RequestMethod.GET)
    public String newUser(Model model) {
        model.addAttribute("user", new User());
        return "users/new";
    }

    @RequestMapping(value = "/new", method = RequestMethod.POST)
    public String postUser(@ModelAttribute User user, Model model) {
        user.setRoles("ROLE_USER");
        user.setPassword(shaPasswordEncoder.encodePassword(user.getPassword(), ""));
        User savedUser = userService.save(user);
        model.addAttribute("user", savedUser);
        return "users/show";
    }

    @RequestMapping(value = "/{userId}", method = RequestMethod.GET)
    public String showUser(@PathVariable("userId") Long userId, Model model) {
        User user = userService.findOne(userId);
        model.addAttribute("user", user);
        return "users/show";
    }

    @RequestMapping(value="/getGenericException",method=RequestMethod.GET)
    public String getGenericException(Model model) throws Exception{
        throw new Exception("Example message");
    }

    @InitBinder
    public void initBinder(WebDataBinder binder){
        binder.registerCustomEditor(Date.class,
                new CustomDateEditor(new SimpleDateFormat("yyyy-MM-dd"), true, 10));
    }

}
